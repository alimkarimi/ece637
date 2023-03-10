#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"
#include "ConnectedNeighbors.h"
#include "ConnectedSet.h"

void error(char *name);

//void Segmentation(struct pixel s, uint8_t **img, int width, int height, double T, int set_count, uint8_t **seg);

// int ConnectedSet_Segmentation (struct pixel s, double T, uint8_t **img, int width, int height, int ClassLabel, 
// uint8_t **seg, int *NumConPixels);

int main(int argc, char **argv){
   
    FILE *fp; //pointer to input image
    struct TIFF_img input_img, output_img; //struct to get dimensions of input image
    double T = 1.0;
    
    /* START PROCESS TO GET IMAGE FROM HARD DRIVE*/
    if ( argc != 2 ) error( argv[0] );

    /* open image file */
    if ( ( fp = fopen ( argv[1], "rb" ) ) == NULL ) {
        fprintf ( stderr, "cannot open file %s\n", argv[1] );
        exit ( 1 );
    }

    /* read image */
    if ( read_TIFF ( fp, &input_img ) ) {
        fprintf ( stderr, "error reading file %s\n", argv[1] );
        exit ( 1 );
    }

    /* close image file */
    fclose ( fp );

    /* check the type of image data */
    if ( input_img.TIFF_type != 'g' ) {
        fprintf ( stderr, "error:  image must be 24-bit color\n" );
        exit ( 1 );}

    get_TIFF( &output_img, input_img.height, input_img.width, 'g' );

    printf("input_img.height is %i\n", input_img.height);
    printf("input_img.width is %i\n", input_img.width);
    int width = input_img.width;
    int height = input_img.height;
    //this loop makes every pixel black. We assume nothing belongs in the segmented region at first
    for ( int i = 0; i < input_img.height; i++ )
        for ( int j = 0; j < input_img.width; j++ ) {
            output_img.mono[i][j] = 0;
        }

    int ClassLabel = 1;
    int NumConPixels = 0;

    for ( int i = 0; i < height; i++ ){
        for ( int j = 0; j < width; j++ ) {
                //printf("In i = %i, j = %i\n", i, j);
                if (output_img.mono[i][j] == 0) { //this means we have not searched this pixel
                    struct pixel x;
                    x.n = j;
                    x.m = i;
                    output_img.mono[i][j] = ClassLabel;
                    ConnectedSet(x, T, input_img.mono, width, height, ClassLabel, output_img.mono, &NumConPixels);
                    
                    if (NumConPixels > 100){ //this is a large set
                        //printf("NumConPixels > 100 where NumConPixels is %i, ClassLabel=%i, i is %i and j is %i\n", NumConPixels, ClassLabel, i, j);
                        ClassLabel++;
                        NumConPixels = 0;


                        }

                    else { //for a small set, set all the values to 255 since the problem mentions there will not be 255 classes
                        for (int i = 0; i< input_img.height; i++)
                            for (int j = 0; j < input_img.width; j++){
                                if (output_img.mono[i][j] == ClassLabel){
                                    output_img.mono[i][j] = 255;
                                }}
                        NumConPixels = 0;
                    }
                }
    }
    }

    //for the values that were set to 255 previously, set them to 0.
    for (int i = 0; i< input_img.height; i++)
        for (int j = 0; j < input_img.width; j++){
            if (output_img.mono[i][j] == 255){
                output_img.mono[i][j] = 0;
            }
        }

    //print out number of classes for segmented image.
    printf("classlabel count=%d for T=%f\n", ClassLabel, T);
    //write image to disk after segmenting output_img.mono
    /* open color image file */
    if ( ( fp = fopen ( "segmentation_output_T_1.tif", "wb" ) ) == NULL ) {
        fprintf ( stderr, "cannot open file color.tif\n");
        exit ( 1 );
    }
    
  /* write color image */
    if ( write_TIFF ( fp, &output_img ) ) {
        fprintf ( stderr, "error writing TIFF file %s\n", argv[2] );
        exit ( 1 );
    }
    
  /* close color image file */
    fclose ( fp );

    /* de-allocate space which was used for the images */
    free_TIFF ( &(input_img) );
    free_TIFF ( &(output_img) );

    return 0;
}
    
    void error(char *name)
{
    printf("usage:  %s  image.tiff \n\n",name);
    printf("this program reads in a 24-bit color TIFF image.\n");
    printf("It then horizontally filters the green component, adds noise,\n");
    printf("and writes out the result as an 8-bit image\n");
    printf("with the name 'green.tiff'.\n");
    printf("It also generates an 8-bit color image,\n");
    printf("that swaps red and green components from the input image");
    exit(1);
}
