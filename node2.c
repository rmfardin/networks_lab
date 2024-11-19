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

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dt2.costs[i][j] = 999;
        }
    }

    dt2.costs[0][0] = 3;   
    dt2.costs[1][1] = 1;   
    dt2.costs[2][2] = 0;   
    dt2.costs[3][3] = 2;   

    struct rtpkt packet;
    for (int i = 0; i < ROW_SIZE; i++) {
        if (i != NODE_ID && dt2.costs[i][i] < 999) { 
            packet.sourceid = NODE_ID;
            packet.destid = i;
            for (int j = 0; j < ROW_SIZE; j++) {
                packet.mincost[j] = dt2.costs[j][j];
            }
            tolayer2(packet);
        }
    }
}

void rtupdate2(rcvdpkt) struct rtpkt *rcvdpkt;
{
    int src = rcvdpkt->sourceid;
    bool update = false;

    for(int i = 0; i < ROW_SIZE; i++){
        
        int new_cost = dt2.costs[src][src] + rcvdpkt->mincost[i];
        if (new_cost < dt2.costs[i][src]){
            dt2.costs[i][src] = new_cost;
            update = true;
        }
    }

    int min_array[4];
    for (int i = 0; i < ROW_SIZE; i++) {
        min_array[i] = 999;

        for (int j = 0; j < 4; j++) { // Via node j
            if (dt2.costs[i][j] < min_array[i]) {
                min_array[i] = dt2.costs[i][j];
            }
        }
    }

    if (update){
        struct rtpkt new_pkt;
        new_pkt.sourceid = NODE_ID;

        for (int i =0; i< ROW_SIZE; i++){
            new_pkt.mincost[i] = min_array[i];
        }

        for (int i = 0; i < ROW_SIZE; i++) {
            if (i != NODE_ID && dt2.costs[i][i] < 999) { 
                new_pkt.destid = i;
                tolayer2(new_pkt);
            }
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
