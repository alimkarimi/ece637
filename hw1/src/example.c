#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"

int add(int p, int q);

int main() {
   printf("Hello, World!");

   int a = 4;
   int b = 2;
   int sum = add(a,b);
   printf("%d", sum);

   return 0;
}

int add(int p, int q ) {
   
    int c = 0;
    c = p + q;
    printf("%d", c);
    return c; 
}