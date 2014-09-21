#include<stdio.h>
#include<stdlib.h>
/* Generic message template. */
enum Msgtypes{
		JOINREQ,			
		JOINREP,
		HEARTBEATMSG,
		DUMMYLASTMSGTYPE
};
typedef struct messagehdr{ 	
	enum Msgtypes msgtype;
} messagehdr;

typedef struct address{
	char addr[6];
} address;
typedef struct memberShipList {
		//struct address membernodes[EN_GPSZ]; 	// to store list of ip address of nodes in the membership list
	 	int lastupdatetime;			// to store the last update time for each corresponding node in node
	 	struct address memnodeip;			// to store the last update time for each corresponding node in node
	 	int peernodeid;			// to store the last update time for each corresponding node in node
		int toclean;							// This is set once the node elapses tfail time ; marked for deletion
		int heartbeatcounter;
				
} memberShipList;

typedef struct member{            
        struct address addr;            // my address
        int inited;                     // boolean indicating if this member is up
        int ingroup;                    // boolean indiciating if this member is in the group
        //queue inmsgq;                   // queue for incoming messages
        int bfailed;                    // boolean indicating if this member has failed
        int nodenumber;                    // boolean indicating if this member has failed
		struct memberShipList mplist[100];	 // This stores the membership list
} member;

void main(){
    static char s[1024];
member *node;
node=malloc(sizeof(member));
memcpy(&node->addr,"10000",sizeof(10000));
    messagehdr *msg;
        size_t msgsize = sizeof(messagehdr) + sizeof(address)+ sizeof(int);
       //size_t msgsize = sizeof(messagehdr) + sizeof(address);

    msg=malloc(msgsize);
    msg->msgtype=JOINREQ;
    memcpy((char *)(msg+1), &node->addr, sizeof(address));
	memcpy((int *)(msg+1+sizeof(address)),&node->nodenumber, sizeof(int));


}

