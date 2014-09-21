/**********************
*
* Progam Name: MP1. Membership Protocol.
* 
* Code authors: <your name here>
*
* Current file: mp2_node.h
* About this file: Header file.
* 
***********************/

#ifndef _NODE_H_
#define _NODE_H_

#define MAX_NODES 100
#include "stdincludes.h"
#include "params.h"
#include "queue.h"
#include "requests.h"
#include "emulnet.h"

/* Configuration Parameters */
char JOINADDR[30];                    /* address for introduction into the group. */
extern char *DEF_SERVADDR;            /* server address. */
extern short PORTNUM;                /* standard portnum of server to contact. */
extern int UNID;                /* UNIQUE NODE ID  */
/* Miscellaneous Parameters */
extern char *STDSTRING;
//shyam
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
        queue inmsgq;                   // queue for incoming messages
        int bfailed;                    // boolean indicating if this member has failed
        int nodenumber;                    // boolean indicating if this member has failed
		struct memberShipList mplist[MAX_NODES];	 // This stores the membership list
} member;

/* Message types */
/* Meaning of different message types
  JOINREQ - request to join the group
  JOINREP - replyto JOINREQ
  HEARTBEAT -  periodic msg sent from nodes after tgossip interval
*/
enum Msgtypes{
		JOINREQ,			
		JOINREP,
		HEARTBEATMSG,
		DUMMYLASTMSGTYPE
};

/* Generic message template. */
typedef struct messagehdr{ 	
	enum Msgtypes msgtype;
} messagehdr;


/* Functions in mp2_node.c */

/* Message processing routines. */
STDCLLBKRET Process_joinreq STDCLLBKARGS;
STDCLLBKRET Process_joinrep STDCLLBKARGS;
STDCLLBKRET Process_heartbeat STDCLLBKARGS;

/*
int recv_callback(void *env, char *data, int size);
int init_thisnode(member *thisnode, address *joinaddr);
*/

/*
Other routines.
*/

void nodestart(member *node, char *servaddrstr, short servport);
void nodeloop(member *node);
int recvloop(member *node);
int finishup_thisnode(member *node);
char *gettimestamp();

#endif /* _NODE_H_ */

