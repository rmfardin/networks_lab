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
} dt2;

#define ROW_SIZE 4
#define COL_SIZE 4

#define NODE_ID 2

extern void tolayer2(struct rtpkt packet);

/*
returns the minimum distance between node NODE_ID and node row_idx
if col_idx is not NULL, it will be set to the index of the node
from which the shortest path is taken. If set to null, it is ignored.
*/

/* students to write the following two routines, and maybe some others */

void rtinit2() {

    for (int i =0; i< ROW_SIZE; i++){
        if(i == NODE_ID){
            continue;
        }

        struct rtpkt temp;
        temp.sourceid = NODE_ID;
        temp.destid = i;
        temp.mincost[0] = 3;
        temp.mincost[1] = 1;
        temp.mincost[2] = 0;
        temp.mincost[3] = 2;
        tolayer2(temp);

    }
}

void rtupdate2(rcvdpkt) struct rtpkt *rcvdpkt;
{
    int src = rcvdpkt->sourceid;
    bool update = false;

    for(int i = 0; i < ROW_SIZE; i++){
        if (i == NODE_ID){
            continue;
        }

        int new_cost = dt2.costs[src][NODE_ID] + rcvdpkt->mincost[i];
        if (new_cost < dt2.costs[NODE_ID][i]){
            dt2.costs[NODE_ID][src] = new_cost;
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
                new_pkt.mincost[j] = dt2.costs[NODE_ID][j];
            }

            tolayer2(new_pkt);
        }
    }
}

printdt2(dtptr) struct distance_table *dtptr;

{
    printf("\n                via     \n");
    printf("   D2 |    0     1    3 \n");
    printf("  ----|-----------------\n");
    printf("     0|  %3d   %3d   %3d\n", dtptr->costs[0][0], dtptr->costs[0][1], dtptr->costs[0][3]);
    printf("dest 1|  %3d   %3d   %3d\n", dtptr->costs[1][0], dtptr->costs[1][1], dtptr->costs[1][3]);
    printf("     3|  %3d   %3d   %3d\n", dtptr->costs[3][0], dtptr->costs[3][1], dtptr->costs[3][3]);
}
