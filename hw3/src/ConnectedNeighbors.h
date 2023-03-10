#ifndef CONNECTEDNEIGHORS_H
#define CONNECTEDNEIGHORS_H
#include <stdint.h>

struct pixel{
    int m, n; 
};

void ConnectedNeighbors(struct pixel s, double T, uint8_t **img, int width, int height, int *M, struct pixel c[4]); //, int width , int height , int *M ); 
/* //NOTE: we must write out struct pixel s when defining the prototype.*/
/*furthermore, we need to define the struct pixel as done above before writing the function prototype that accepts the struct*/

#endif
