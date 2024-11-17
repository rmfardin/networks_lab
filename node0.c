#include <stdio.h>
#include <stdbool.h>


struct rtpkt {
    int sourceid;   /* id of sending router sending this pkt */
    int destid;     /* id of router to which pkt being sent
                       (must be an immediate neighbor) */
    int mincost[4]; /* min cost to node 0 ... 3 */
};

extern int TRACE;
extern int YES;
extern int NO;

struct dt0 {
    int costs[4][4];
} dt0;

#define ROW_SIZE 4
#define COL_SIZE 4

#define NODE_ID 0

extern void tolayer2(struct rtpkt packet);

/* students to write the following two routines, and maybe some others */

void rtinit0() {

    for (int i =0; i< ROW_SIZE; i++){
        if(i == NODE_ID){
            continue;
        }

        struct rtpkt temp;
        temp.sourceid = NODE_ID;
        temp.destid = i;
        temp.mincost[0] = 0;
        temp.mincost[1] = 1;
        temp.mincost[2] = 3;
        temp.mincost[3] = 7;
        tolayer2(temp);

    }
}

void rtupdate0(rcvdpkt) struct rtpkt *rcvdpkt;
{
    int src = rcvdpkt->sourceid;
    bool update = false;

    for(int i = 0; i < ROW_SIZE; i++){
        if (i == NODE_ID){
            continue;
        }

        int new_cost = dt0.costs[src][NODE_ID] + rcvdpkt->mincost[i];
        if (new_cost < dt0.costs[NODE_ID][i]){
            dt0.costs[NODE_ID][src] = new_cost;
            update = true;
        }
    }

    if (update){
        for (int i =0; i< ROW_SIZE; i++){
            if(i == NODE_ID){
                continue;
            }

            struct rtpkt new_pkt;
            new_pkt.sourceid = NODE_ID;
            new_pkt.destid = i;

            for (int j = 0; j < ROW_SIZE; j++) {
                new_pkt.mincost[j] = dt0.costs[NODE_ID][j];
            }

            tolayer2(new_pkt);
        }
    }
}

printdt0(dtptr) struct dt0 *dtptr;

{
    printf("\n                via     \n");
    printf("   D0 |    1     2    3 \n");
    printf("  ----|-----------------\n");
    printf("     1|  %3d   %3d   %3d\n", dtptr->costs[1][1], dtptr->costs[1][2], dtptr->costs[1][3]);
    printf("dest 2|  %3d   %3d   %3d\n", dtptr->costs[2][1], dtptr->costs[2][2], dtptr->costs[2][3]);
    printf("     3|  %3d   %3d   %3d\n", dtptr->costs[3][1], dtptr->costs[3][2], dtptr->costs[3][3]);
}

linkhandler0(linkid, newcost) int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */

{}
