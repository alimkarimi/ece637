
#include <math.h>
#include "tiff.h"
#include "allocate.h"
#include "randlib.h"
#include "typeutil.h"

void error(char *name);

int main (int argc, char **argv) 
{
  FILE *fp;
  struct TIFF_img input_img, green_img, color_img, red_img, my_color_img;
  double **imgred0, **imggreen1, **imgblue2;
  int32_t i, j;
  


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
  // img1 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  // img2 = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  imgred0 = (double **)get_img(input_img.width +8,input_img.height +8,sizeof(double));
  imggreen1 = (double **)get_img(input_img.width+8,input_img.height+8 ,sizeof(double));
  imgblue2 = (double **)get_img(input_img.width+8,input_img.height+8,sizeof(double));


  printf("after allocation\n");


  printf("made it here before 0 padded images\n");
  
  /* create 0 padded blue image*/

  for (i = 0; i < input_img.height + 8; i++)
    for (j = 0; j < input_img.width + 8; j++){
      /*printf("we are at the idx i=%i, j=%i\n", i, j);*/
      if (i >=4  && j >= 4 && i <= (input_img.height + 4) && j <= (input_img.width + 4)) {
        /*rintf("bluedpad: we are in the condition for i=%i and j=%i!\n", i, j); */
        imgred0[i][j] = input_img.color[0][i-4][j-4];
        imggreen1[i][j] = input_img.color[1][i-4][j-4];
        imgblue2[i][j] =  input_img.color[2][i-4][j-4];

       /* //printf("img0padblue[%i][%i] is %f\n", i, j, img0padblue[i][j]); */
      }
      else {
        imgred0[i][j] = 0;
        imggreen1[i][j] = 0;
        imgblue2[i][j] =  0;


      }
    }

   printf("made it here");

 
  double sum_red = 0;
  double sum_green =0;
  double sum_blue = 0;
  /* filter red image using filter defined in filter[9][9]*/
  
  for (i = 0; i < input_img.height; i++)
    for ( j = 0; j < input_img.width; j++) {
      sum_red = 0.0;
      sum_green = 0.0;
      sum_blue = 0.0;
      for (int k = -4; k <= 4; k++)
        for (int  m = -4; m <= 4; m++) {
          
            sum_red = sum_red + (imgred0[i-k+4][j-m+4] / 81.0);
            sum_green = sum_green + (imggreen1[i-k+4][j-m+4] / 81.0) ;
            sum_blue = sum_blue + (imgblue2[i-k+4][j-m+4] / 81.0);
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


printf("but did i make it here?\n");

/*set up structure for MY output achromatic image*/
/* to allocate a full color image use type 'c'*/
get_TIFF (&red_img, input_img.height, input_img.width, 'g');

/* set up structure for MY output color image*/
/* note that 'c' is needed not 'g'*/
get_TIFF (&my_color_img, input_img.height, input_img.width, 'c');

/* copy RED (my filtered image) components to new images*/
// for (i = 0; i < input_img.height; i++)
// for (j = 0; j < input_img.width; j++) {
//   pixel = (int32_t)imgred0[i][j];
//   if (pixel>255) {
//     red_img.mono[i][j] = 255; //clip
//   }
//   else {
//     if (pixel < 0) red_img.mono[i][j] = 0; //clip
//     else red_img.mono[i][j] = pixel;
//   }
// }


  // /* Filter image along horizontal direction */
  // for ( i = 0; i < input_img.height; i++ )
  // for ( j = 1; j < input_img.width-1; j++ ) {
  //   img2[i][j] = (img1[i][j-1] + img1[i][j] + img1[i][j+1])/3.0;
  // }

  /* Fill in boundary pixels */
  // for ( i = 0; i < input_img.height; i++ ) {
  //   img2[i][0] = 0; 
  //   img2[i][input_img.width-1] = 0;
  // }

  /* Set seed for random noise generator */
  //srandom2(1);

  /* Add noise to image */
  // for ( i = 0; i < input_img.height; i++ )
  // for ( j = 1; j < input_img.width-1; j++ ) {
  //   img2[i][j] += 32*normal();
  // }

  /* set up structure for output achromatic image */
  /* to allocate a full color image use type 'c' */
  get_TIFF ( &green_img, input_img.height, input_img.width, 'g' );
    
  /* set up structure for output color image */
  /* Note that the type is 'c' rather than 'g' */
  get_TIFF ( &color_img, input_img.height, input_img.width, 'c' );

  /* copy green component to new images */
  // for ( i = 0; i < input_img.height; i++ )
  // for ( j = 0; j < input_img.width; j++ ) {
  //   pixel = (int32_t)img2[i][j];
  //   if(pixel>255) {
  //     green_img.mono[i][j] = 255;
  //   }
  //   else {
  //     if(pixel<0) green_img.mono[i][j] = 0;
  //     else green_img.mono[i][j] = pixel;
  //   }
  // }

  /* open MY RED image file*/
  if ( ( fp = fopen ( "red.tif", "wb" ) ) == NULL ) {
    fprintf ( stderr, "cannot open file green.tif\n");
    exit ( 1 );    
  }

  /*write MY RED image file*/
  if ( write_TIFF ( fp, &red_img ) ) {
    fprintf ( stderr, "error writing TIFF file %s\n", argv[2] );
    exit ( 1 );
  }

   /* close MY RED image file */
  fclose ( fp );

  /* copy professor's illustration of construction a sample color image! */
  for ( i = 0; i < input_img.height; i++ )
      for ( j = 0; j < input_img.width; j++ ) {
        my_color_img.color[0][i][j] = imgred0[i][j];
        my_color_img.color[1][i][j] = imggreen1[i][j];
        my_color_img.color[2][i][j] = imgblue2[i][j];
      }
  
  
  /* open, write, and close MY COLOR image file*/
  /* open color image file */
  if ( ( fp = fopen ( "MY_color.tif", "wb" ) ) == NULL ) {
      fprintf ( stderr, "cannot open file color.tif\n");
      exit ( 1 );
  }
    
  /* write color image */
  if ( write_TIFF ( fp, &my_color_img ) ) {
      fprintf ( stderr, "error writing TIFF file %s\n", argv[2] );
      exit ( 1 );
  }
    
  /* close color image file */
  fclose ( fp );


  /* Illustration: constructing a sample color image -- interchanging the red and green components from the input color image */
  for ( i = 0; i < input_img.height; i++ )
      for ( j = 0; j < input_img.width; j++ ) {
          color_img.color[0][i][j] = input_img.color[1][i][j];
          color_img.color[1][i][j] = input_img.color[0][i][j];
          color_img.color[2][i][j] = input_img.color[2][i][j];
      }

  /* open green image file */
  if ( ( fp = fopen ( "green.tif", "wb" ) ) == NULL ) {
    fprintf ( stderr, "cannot open file green.tif\n");
    exit ( 1 );
  }

  /* write green image */
  if ( write_TIFF ( fp, &green_img ) ) {
    fprintf ( stderr, "error writing TIFF file %s\n", argv[2] );
    exit ( 1 );
  }

  /* close green image file */
  fclose ( fp );
    
    
  /* open color image file */
  if ( ( fp = fopen ( "color.tif", "wb" ) ) == NULL ) {
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
  free_TIFF ( &(green_img) );
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

