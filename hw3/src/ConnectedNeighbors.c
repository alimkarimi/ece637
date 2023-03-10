#include "ConnectedNeighbors.h"
#include "ConnectedSet.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

 


void ConnectedNeighbors(struct pixel s, /* data struct that contains location of pixel s whose connected neighbors will be computed*/
double T, /*threshold*/
 uint8_t **img, /* 2D array of pixels img[m][n] desnoted as xs in |xs - xr| <= T*/
 /* xs is the pix value at lattice pt s */
 int width, int height, /* width and heigh of img[height][width]*/
 int *M, /* an output of function: pointer to number of neighbors connected to pixel s*/
 struct pixel c[4]) { /*an output of function: an array of structs with the c[x].m and c[x].n holding the row/cols of the connected neighbors of s */
    int row = s.m;
    int col = s.n;
    if ((row - 1 >= 0) && (abs(img[row][col] - img[row-1][col]) <= T)) {
        //printf("in top row condition\n");
        c[*M].m = row-1;
        c[*M].n = col;
        (*M)++;
    /* check with bottom pixel. Compute xs - xr < T*/
    }
    if ((row + 1 <= height) && (abs(img[row][col] - img[row+1][col]) <= T)){
        //printf("in bottom row condition");
        c[*M].m = row+1;
        c[*M].n = col;
        (*M)++;
    }
    /* check with left pixel. Compute xs - xr < T*/
    if ((col -1 >=0) && (abs(img[row][col] - img[row][col-1]) <= T)){
        //printf("in left col condition");
        c[*M].m = row;
        c[*M].n = col-1;
        (*M)++;
    }
    /* check with righ pixel. Compute xs - xr < T*/
    if ((col +1 <= width) && (abs(img[row][col] - img[row][col+1]) <= T)){
        //printf("in right col condition");
        //store pixel coords of img in c
        c[*M].m = row;
        c[*M].n = col+1;  
        (*M)++;      
    }
    //printf("in total, the num of connected neighbors are %i\n", *M);
    }

