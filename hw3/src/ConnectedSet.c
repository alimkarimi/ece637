#include "ConnectedNeighbors.h"
#include "ConnectedSet.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void ConnectedSet(struct pixel s, double T, uint8_t **img, int width, int height, int ClassLabel, 
uint8_t **seg, int *NumConPixels){
    //printf("in function in ConnectedSet\n");
    struct pixel c[4]; //each call to ConnectedNeighbors needs to keep track of its own neighbors.
    int M = 0;
    ConnectedNeighbors(s, T, img, width, height, &M, c);
    while (M > 0) { //search connected neighbors stored in c[4] to see which of those are connected to its neighbors
        //first, check if pixel is already labeled
        if (seg[c[M-1].m][c[M-1].n] != ClassLabel){
            //printf("this pixel is a connected component but not yet assigned a value of 0.. we need to assign it to zero\n");
            seg[c[M-1].m][c[M-1].n] = ClassLabel;
            (*NumConPixels)++; //increase the count of connected pixels
            ConnectedSet(c[M-1], T, img, width, height, ClassLabel, seg, NumConPixels);
        }
        //printf("M is %i\n", M);
        //we want to call ConnectedNeighbors on the pixel we found to be connected (i.e, find its connected neighbors)
        M--; //decrement stack - at the most we would have c[3], so we want to get the assign 0 to 
        //the segmentation image pixels that were found to be connected in the
        // ConnectedNeighbors function run (i.e c[2], c[1], or c[0])
        
    }

}
