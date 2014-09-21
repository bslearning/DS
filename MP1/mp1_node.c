/**********************
*
* Progam Name: MP1. Membership Protocol
* 
* Code authors: <your name here>
*
* Current file: mp1_node.c
* About this file: Member Node Implementation
* 
***********************/

#include "mp1_node.h"
#include "emulnet.h"
#include "MPtemplate.h"
#include "log.h"
#include "sys/time.h"
int UNID=0;
/*
 *
 * Routines for introducer and current time.
 *
 */

char NULLADDR[] = {0,0,0,0,0,0};
int isnulladdr( address *addr){
    return (memcmp(addr, NULLADDR, 6)==0?1:0);
}

/* 
Return the address of the introducer member. 
*/
address getjoinaddr(void){

    address joinaddr;

    memset(&joinaddr, 0, sizeof(address));
    *(int *)(&joinaddr.addr)=1;
    *(short *)(&joinaddr.addr[4])=0;

    return joinaddr;
}

/*
 *
 * Message Processing routines.
 *
 */

/* 
Received a JOINREQ (joinrequest) message.
// handled by the lead node and send jon rep
*/
void Process_joinreq(void *env, char *data, int size)
{
// code to get the timestamp recorded of jjjj
struct timeval tp;
gettimeofday(&tp,NULL);
long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

	/* <your code goes in here> */
	messagehdr *msg;
    member *node = (member *) env;
// QUESTION :first 6 bytes : sizeof(addr) has the address : fetch that alone how
    address *destaddr = (char *)data;
	
// env holds the node on which it is called.
	node->mplist[UNID].heartbeatcounter=1;
// copy address to memnodeip iside mplist
	memcpy(&(node->mplist[UNID].memnodeip),destaddr,sizeof(address));
	node->mplist[UNID].lastupdatetime=ms;

	printf("\nSIZE OF MEMLIST %d",sizeof(node->mplist));
// once u get join req from a specific node
// look at your membership list
// update the current timestamp and increment the heartbeat counter for the node from which you received the message

//for i in  number of records of mplist for that source node
// 
//	node->mplist.heartbeatcounter++;
//	node->mplist.
	//creating response with ip:heartbeatcounter:timestamp[ local to the node ] 
	
	
     size_t msgsize = sizeof(messagehdr) + sizeof(address)+sizeof(struct memberShipList);
     //size_t msgsize = sizeof(messagehdr) + sizeof(address);
     msg=malloc(msgsize);
     msg->msgtype=JOINREP;
     //memcpy((char *)(msg+1), &node->addr, sizeof(address));

//form and send response to join req
printf("SENDING from within PROCESS_JOINREQ\n");
        MPp2psend(&node->addr, destaddr, (char *)msg, msgsize);
printf("DONE and value of node is %d",node->nodenumber);

    return;
}

/* 
Received a JOINREP (joinreply) message. 
*/
void Process_joinrep(void *env, char *data, int size)
{
	/* <your code goes in here> */

    return;
}
void Process_heartbeat(void *env, char *data, int size)
{
	/* <your code goes in here> */

    return;
}


/* 
Array of Message handlers. 
*/
void ( ( * MsgHandler [20] ) STDCLLBKARGS )={ /* Message processing operations at the P2P layer. */
    Process_joinreq, 
    Process_joinrep,
	Process_heartbeat
};

/* 
Called from nodeloop() on each received packet dequeue()-ed from node->inmsgq. 
Parse the packet, extract information and process. 
env is member *node, data is 'messagehdr'. 
*/
int recv_callback(void *env, char *data, int size){

    member *node = (member *) env;
    messagehdr *msghdr = (messagehdr *)data;
    char *pktdata = (char *)(msghdr+1);
//    char *pktdata = (char *)(msghdr+7);

    if(size < sizeof(messagehdr)){
#ifdef DEBUGLOG
        LOG(&((member *)env)->addr, "Faulty packet received - ignoring");
#endif
        return -1;
    }

#ifdef DEBUGLOG
    LOG(&((member *)env)->addr, "Received msg type %d with %d B payload", msghdr->msgtype, size - sizeof(messagehdr));
#endif

    if((node->ingroup && msghdr->msgtype >= 0 && msghdr->msgtype <= DUMMYLASTMSGTYPE) || (!node->ingroup && msghdr->msgtype==JOINREP))            
	{            /* if not yet in group, accept only JOINREPs */

			LOG(&((member *)env)->addr, "INSIDE RECV_CALLBACK %d",msghdr->msgtype);
// QUESTION : size-sizeof(messagehdr) ? when addr is copied after the first byte
			MsgHandler[msghdr->msgtype](env, pktdata, size-sizeof(messagehdr));
	}
	
    /* else ignore (garbled message) */


    free(data);

    return 0;

}

/*
 *
 * Initialization and cleanup routines.
 *
 */

/* 
Find out who I am, and start up. 
*/
int init_thisnode(member *thisnode, address *joinaddr){
    
    if(MPinit(&thisnode->addr, PORTNUM, (char *)joinaddr)== NULL){ /* Calls ENInit */
#ifdef DEBUGLOG
        LOG(&thisnode->addr, "MPInit failed");
#endif
        exit(1);
    }
#ifdef DEBUGLOG
    else LOG(&thisnode->addr, "MPInit succeeded. Hello.");
#endif

    thisnode->bfailed=0;
    thisnode->inited=1;
    thisnode->ingroup=0;
    /* node is up! */

    return 0;
}


/* 
Clean up this node. 
*/
int finishup_thisnode(member *node){

	/* <your code goes in here> */
    return 0;
}


/* 
 *
 * Main code for a node 
 *
 */

/* 
Introduce self to group. 
*/
int introduceselftogroup(member *node, address *joinaddr){
    
    messagehdr *msg;
#ifdef DEBUGLOG
    static char s[1024];
#endif

    if(memcmp(&node->addr, joinaddr, 4*sizeof(char)) == 0){
        /* I am the group booter (first process to join the group). Boot up the group. */
#ifdef DEBUGLOG
        LOG(&node->addr, "Starting up group...");
#endif

        node->ingroup = 1;
		UNID=0;
		node->nodenumber=UNID;

    }
    else{
		node->nodenumber=UNID;
        size_t msgsize = sizeof(messagehdr) + sizeof(address)+ sizeof(int);
       //size_t msgsize = sizeof(messagehdr) + sizeof(address);

        msg=malloc(msgsize);
printf("SIZE OF MSG %d:",msgsize);

    /* create JOINREQ message: format of data is {struct address myaddr} */
        msg->msgtype=JOINREQ;
        memcpy((char *)(msg+1), &node->addr, sizeof(address));
// try to have node id also sent  // fails here
		memcpy((char *)(msg+7),&node->nodenumber, sizeof(int));

#ifdef DEBUGLOG
        sprintf(s, "Trying to join...MSG");
        LOG(&node->addr, s);
#endif

    /* send JOINREQ message to introducer member. */

        MPp2psend(&node->addr, joinaddr, (char *)msg, msgsize);
        
        free(msg);
        sprintf(s, "Trying to join... COMPLETED");
		// give the node an id
        LOG(&node->addr, s);
    }
		// every node get a unique number : ID
		UNID++;

    return 1;

}

/* 
Called from nodeloop(). 
*/
void checkmsgs(member *node){
    void *data;
    int size;
    static char s[1024];

    /* Dequeue waiting messages from node->inmsgq and process them. */
//printf("how do i print node address. 	");
    while((data = dequeue(&node->inmsgq, &size)) != NULL) {
		printf("\n-------------- ************************************************************  ADDR INSIDE  checkMSG for node :%d: address",node->addr);
        recv_callback((void *)node, data, size); 
    }
		//printf("\n--------------INSIDE  checkMSG DONE ******");
    return;
}


/* 
Executed periodically for each member. 
Performs necessary periodic operations. 
Called by nodeloop(). 
*/
void nodeloopops(member *node){

	/* <your code goes in here> */
//shyam


    static char s[1024];
    sprintf(s, "NODELOOPOPS called for %d",node->addr);
     LOG(&node->addr, s);

//        size_t msgsize = sizeof(messagehdr) + sizeof(address);
//        msg=malloc(msgsize);
//
//    /* create JOINREQ message: format of data is {struct address myaddr} */
//        msg->msgtype=JOINREQ;
//        memcpy((char *)(msg+1), &node->addr, sizeof(address));
//
//#ifdef DEBUGLOG
//        sprintf(s, "Trying to join...");
//        LOG(&node->addr, s);
//#endif
//
//    /* send JOINREQ message to introducer member. */
//        MPp2psend(&node->addr, joinaddr, (char *)msg, msgsize);
//        
//        free(msg);
//        MsgHandler[2](env, pktdata, size-sizeof(messagehdr));
    return;
}

/* 
Executed periodically at each member. Called from app.c.
*/
void nodeloop(member *node){

printf("\nNODELOOP CALLED FOR %d",node->nodenumber);
    static char s[1024];
    //printf("ENTEREED NODELOOPOPS called for %d <-Address \n",node->addr);

    if (node->bfailed) return;


    checkmsgs(node);

    /* Wait until you're in the group... */
    if(!node->ingroup) return ;

    /* ...then jump in and share your responsibilites! */
   // sprintf(s, "BEFORE NODELOOPOPS called for ");
     LOG(&node->addr, s); // always passing first one address ?
    nodeloopops(node);
    
    return;
}

/* 
All initialization routines for a member. Called by app.c. 
*/
void nodestart(member *node, char *servaddrstr, short servport){

    address joinaddr=getjoinaddr();

    /* Self booting routines */
    if(init_thisnode(node, &joinaddr) == -1){

#ifdef DEBUGLOG
        LOG(&node->addr, "init_thisnode failed. Exit.");
#endif
        exit(1);
    }

    if(!introduceselftogroup(node, &joinaddr)){
        finishup_thisnode(node);
#ifdef DEBUGLOG
        LOG(&node->addr, "Unable to join self to group. Exiting.");
#endif
        exit(1);
    }

    return;
}

/* 
Enqueue a message (buff) onto the queue env. 
*/
int enqueue_wrppr(void *env, char *buff, int size){    return enqueue((queue *)env, buff, size);}

/* 
Called by a member to receive messages currently waiting for it. 
*/
int recvloop(member *node){
    if (node->bfailed) return -1;
    else return MPrecv(&(node->addr), enqueue_wrppr, NULL, 1, &node->inmsgq); 
    /* Fourth parameter specifies number of times to 'loop'. */
}

