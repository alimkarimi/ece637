
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
  int32_t i, j, k, m;

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

  imgred0 = (double **)get_img(input_img.width +4,input_img.height +4,sizeof(double));
  imggreen1 = (double **)get_img(input_img.width+4,input_img.height+4 ,sizeof(double));
  imgblue2 = (double **)get_img(input_img.width+4,input_img.height+4,sizeof(double));
  get_TIFF( &color_img, input_img.height, input_img.width, 'c' );

  
  /* create 0 padded images */
  for (i = 0; i < input_img.height + 4; i++)
    for (j = 0; j < input_img.width + 4; j++){

      printf("i is %i and j is %i\n", i , j );
      if (i >4  && j > 4 && i < (input_img.height + 2) && j < (input_img.width + 2)) {
 
        imgred0[i][j] = input_img.color[0][i-2][j-2];
        imggreen1[i][j] = input_img.color[1][i-2][j-2];
        imgblue2[i][j] =  input_img.color[2][i-2][j-2];
      }
      else {

        imgred0[i][j] = 0;
        imggreen1[i][j] = 0;
        imgblue2[i][j] =  0;


      }

    }

  double sum_red;
  double sum_green;
  double sum_blue;
  /* filter image */
  double lambda = 1.5;
  double h = 0.04; //value of h(m,n)
  double outer = -h * lambda; //value of g(m,n)'s outer pixels
  double inner = 1 + (lambda - (h * lambda)); //value of g(m,n)'s middle pixel because of the delta function. 


  for (i = 0; i < input_img.height; i++) {
    for ( j = 0; j < input_img.width; j++) {
      sum_red = 0.0;
      sum_green = 0.0;
      sum_blue = 0.0;
      for (k = -2; k <= 2; k++)
        for ( m = -2; m <= 2; m++) {
            if (k == 0 && m == 0) {
            sum_red = sum_red + (imgred0[i-k+2][j-m+2] * inner);
            sum_green = sum_green + (imggreen1[i-k+2][j-m+2] * inner);
            sum_blue = sum_blue + (imgblue2[i-k+2][j-m+2] * inner);
            }
            else {
            sum_red = sum_red + (imgred0[i-k+2][j-m+2] * outer);
            sum_green = sum_green + (imggreen1[i-k+2][j-m+2] * outer);
            sum_blue = sum_blue + (imgblue2[i-k+2][j-m+2] * outer);
            }
            }
      if (sum_red > 255) {
        color_img.color[0][i][j] = 255;
      }
      if (sum_red < 0) {
        color_img.color[0][i][j] = 0;
      }
      else{
        color_img.color[0][i][j] = sum_red;
      }
      if (sum_green > 255) {
        color_img.color[1][i][j] = 255;
      }
      if (sum_green < 0) {
        color_img.color[1][i][j] = 0;
      }
      else{
        color_img.color[1][i][j] = sum_green;
      }
      if (sum_blue> 255) {
        color_img.color[2][i][j] = 255;
      }
      if (sum_blue < 0) {
        color_img.color[2][i][j] = 0;
      }
      else{
        color_img.color[2][i][j] = sum_blue;
      }
      
    }
  }


  /* open color image file */
  if ( ( fp = fopen ( "sharpened_lambda1_5.tif", "wb" ) ) == NULL ) {
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

