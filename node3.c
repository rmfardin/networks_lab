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
} dt3;

#define ROW_SIZE 4
#define COL_SIZE 4

#define NODE_ID 3

extern void tolayer2(struct rtpkt packet);
/* students to write the following two routines, and maybe some others */

void rtinit3() {

    for (int i =0; i< ROW_SIZE; i++){
        if(i == NODE_ID){
            continue;
        }

        struct rtpkt temp;
        temp.sourceid = NODE_ID;
        temp.destid = i;
        temp.mincost[0] = 7;
        temp.mincost[1] = 999;
        temp.mincost[2] = 2;
        temp.mincost[3] = 0;
        tolayer2(temp);

    }
}



void rtupdate3(rcvdpkt) struct rtpkt *rcvdpkt;
{
    int src = rcvdpkt->sourceid;
    bool update = false;

    for(int i = 0; i < ROW_SIZE; i++){
        if (i == NODE_ID){
            continue;
        }

        int new_cost = dt3.costs[src][NODE_ID] + rcvdpkt->mincost[i];
        if (new_cost < dt3.costs[NODE_ID][i]){
            dt3.costs[NODE_ID][src] = new_cost;
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
                new_pkt.mincost[j] = dt3.costs[NODE_ID][j];
            }

            tolayer2(new_pkt);
        }
    }
}

printdt3(dtptr) struct distance_table *dtptr;

{
    printf("\n             via     \n");
    printf("   D3 |    0     2 \n");
    printf("  ----|-----------\n");
    printf("     0|  %3d   %3d\n", dtptr->costs[0][0], dtptr->costs[0][2]);
    printf("dest 1|  %3d   %3d\n", dtptr->costs[1][0], dtptr->costs[1][2]);
    printf("     2|  %3d   %3d\n", dtptr->costs[2][0], dtptr->costs[2][2]);
}
