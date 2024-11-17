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

struct distance_table {
    int costs[4][4];
} dt1;


#define ROW_SIZE 4
#define COL_SIZE 4

#define NODE_ID 1

extern void tolayer2(struct rtpkt packet);
/* students to write the following two routines, and maybe some others */

void rtinit1() {

    for (int i =0; i< ROW_SIZE; i++){
        if(i == NODE_ID){
            continue;
        }

        struct rtpkt temp;
        temp.sourceid = NODE_ID;
        temp.destid = i;
        temp.mincost[0] = 1;
        temp.mincost[1] = 0;
        temp.mincost[2] = 1;
        temp.mincost[3] = 999;
        tolayer2(temp);

    }
}

void rtupdate1(rcvdpkt) struct rtpkt *rcvdpkt;
{
    int src = rcvdpkt->sourceid;
    bool update = false;

    for(int i = 0; i < ROW_SIZE; i++){
        if (i == NODE_ID){
            continue;
        }

        int new_cost = dt1.costs[src][NODE_ID] + rcvdpkt->mincost[i];
        if (new_cost < dt1.costs[NODE_ID][i]){
            dt1.costs[NODE_ID][src] = new_cost;
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
                new_pkt.mincost[j] = dt1.costs[NODE_ID][j];
            }

            tolayer2(new_pkt);
        }
    }
}

printdt1(dtptr) struct distance_table *dtptr;

{
    printf("\n             via   \n");
    printf("   D1 |    0     2 \n");
    printf("  ----|-----------\n");
    printf("     0|  %3d   %3d\n", dtptr->costs[0][0], dtptr->costs[0][2]);
    printf("dest 2|  %3d   %3d\n", dtptr->costs[2][0], dtptr->costs[2][2]);
    printf("     3|  %3d   %3d\n", dtptr->costs[3][0], dtptr->costs[3][2]);
}

linkhandler1(linkid, newcost) int linkid, newcost;
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */

{}
