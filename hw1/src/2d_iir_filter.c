
#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"

void error(char *name);

int main (int argc, char **argv) 
{
  FILE *fp;
  struct TIFF_img input_img, color_img;
  double **imgred0, **imggreen1, **imgblue2;
  int32_t m, n;
  


  printf("done with for loop \n");

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
  if ( input_img.TIFF_type != 'c' ) {
    fprintf ( stderr, "error:  image must be 24-bit color\n" );
    exit ( 1 );
  }
  printf("before allocation\n");
  /* Allocate image of double precision floats */
  imgred0 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  imggreen1 = (double **)get_img(input_img.width,input_img.height ,sizeof(double));
  imgblue2 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  get_TIFF( &color_img, input_img.height, input_img.width, 'c' );

  printf("after allocation\n");
  
  /* create 0 images */
  for (m = 0; m < input_img.height ; m++)
    for (n = 0; n < input_img.width ; n++){

        imgred0[m][n] = 0;
        imggreen1[m][n] = 0;
        imgblue2[m][n] =  0;
      }


  /* filter image */
  
  for (m = 0; m < input_img.height; m++) {
    for (n = 0; n< input_img.width; n++) {

    if (m == 0 && n == 0){
        imgred0[m][n] = input_img.color[0][m][n];
        imggreen1[m][n] = input_img.color[1][m][n];
        imgblue2[m][n] = input_img.color[2][m][n];
        
    }
    else if (m == 0 && n > 0){
        /* we are in condition for y(m-1), n, cannot compute y(m-1, n), so, we must only compute y(m, n-1)*/
        /* need to compute 0.01x * x(m,n) + 0.9 * y(m, n-1)*/
        printf("in m = 0, n \n");
        imgred0[m][n] = (0.01 * input_img.color[0][m][n]) + 0.9 * imgred0[m][n-1];
        imggreen1[m][n] = (0.01 * input_img.color[1][m][n]) + 0.9 * imggreen1[m][n-1];
        imgblue2[m][n] = (0.01 * input_img.color[2][m][n]) + 0.9 * imgblue2[m][n-1];

        
    }
    else if (m > 0 && n == 0){
        /* we are in condition for y(m, n-1), which computes to 0, so, we must only compute y(m-1, n)*/
        /* need to compute 0.01 * x(m,n) + 0.9 * y(m-1, n)*/
        printf("in m >0, n = 0\n");
        imgred0[m][n] = (0.01 * (double)input_img.color[0][m][n]) + (0.9 * imgred0[m-1][n]);
        imggreen1[m][n] = (0.01 * input_img.color[1][m][n]) + (0.9 * imggreen1[m-1][n]);
        imgblue2[m][n] = (0.01 * input_img.color[2][m][n]) + (0.9 * imgblue2[m-1][n]);
        
    }
    else {
        /* we are condition where neither m nor n are going to be out of bounds of the image. Therefore, we can compute the entire */
        /* difference equation*/
;
        
        imgred0[m][n]  = (0.01* input_img.color[0][m][n]) + (0.9 * (imgred0[m-1][n] + imgred0[m][n-1]) )
        - (0.81 * imgred0[m-1][n-1]);

        imggreen1[m][n] = (0.01* input_img.color[1][m][n]) + (0.9 * (imggreen1[m-1][n] + imggreen1[m][n-1]) )
        - (0.81 * imggreen1[m-1][n-1]);

        imgblue2[m][n] = (0.01* input_img.color[2][m][n]) + (0.9 * (imgblue2[m-1][n] + imgblue2[m][n-1]))
        - (0.81 * imgblue2[m-1][n-1]);

    }
    printf("starting if statements\n");
      if (imgred0[m][n]  > 255) {
        color_img.color[0][m][n] = 255;
      }
      if (imgred0[m][n]  < 0) {
        color_img.color[0][m][n] = 0;
      }
      else{
        color_img.color[0][m][n] = imgred0[m][n] ;
      }
      if (imggreen1[m][n] > 255) {
        color_img.color[1][m][n] = 255;
      }
      if (imggreen1[m][n] < 0) {
        color_img.color[1][m][n] = 0;
      }
      else{
        color_img.color[1][m][n] = imggreen1[m][n];
      }
      if ( imgblue2[m][n]> 255) {
        color_img.color[2][m][n] = 255;
      }
      if ( imgblue2[m][n] < 0) {
        color_img.color[2][m][n] = 0;
      }
      else{
        color_img.color[2][m][n] =  imgblue2[m][n];
      }
      
    }
  }


  /* open color image file */
  if ( ( fp = fopen ( "iir_filt_output.tif", "wb" ) ) == NULL ) {
      fprintf ( stderr, "cannot open file color.tif\n");
      exit ( 1 );
  }
    
  /* write color image */
  if ( write_TIFF ( fp, &color_img ) ) {
      fprintf ( stderr, "error writing TIFF file %s\n", argv[2] );
      exit ( 1 );
  }
    
  /* close color image file */
  fclose ( fp );

  /* de-allocate space which was used for the images */
  free_TIFF ( &(input_img) );
  free_TIFF ( &(color_img) );
  
  // free_img( (void**)img1 );
  // free_img( (void**)img2 );
  free_img(  (void**)imgred0);  
  free_img(  (void**)imggreen1); 
  free_img(  (void**)imgblue2); 
  return(0);
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

