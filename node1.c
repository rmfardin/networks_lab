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

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dt1.costs[i][j] = 999;
        }
    }

    dt1.costs[0][0] = 1;   
    dt1.costs[1][1] = 0;   
    dt1.costs[2][2] = 1;   
    dt1.costs[3][3] = 999;   

    struct rtpkt packet;
    for (int i = 0; i < ROW_SIZE; i++) {
        if (i != NODE_ID && dt1.costs[i][i] < 999) { 
            packet.sourceid = NODE_ID;
            packet.destid = i;
            for (int j = 0; j < ROW_SIZE; j++) {
                packet.mincost[j] = dt1.costs[j][j];
            }
            tolayer2(packet);
        }
    }
}

void rtupdate1(rcvdpkt) struct rtpkt *rcvdpkt;
{
    int src = rcvdpkt->sourceid;
    bool update = false;

    for(int i = 0; i < ROW_SIZE; i++){
        
        int new_cost = dt1.costs[src][src] + rcvdpkt->mincost[i];
        if (new_cost < dt1.costs[i][src]){
            dt1.costs[i][src] = new_cost;
            update = true;
        }
    }

    int min_array[4];
    for (int i = 0; i < ROW_SIZE; i++) {
        min_array[i] = 999;

        for (int j = 0; j < 4; j++) { // Via node j
            if (dt1.costs[i][j] < min_array[i]) {
                min_array[i] = dt1.costs[i][j];
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
            if (i != NODE_ID && dt1.costs[i][i] < 999) { 
                new_pkt.destid = i;
                tolayer2(new_pkt);
            }
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
