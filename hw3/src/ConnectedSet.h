#ifndef CONNECTEDSET_H
#define CONNECTEDSET_H
#include <stdint.h>

void ConnectedSet (struct pixel s, double T, uint8_t **img, int width, int height, int ClassLabel, 
uint8_t **seg, int *NumConPixels);

#endif
