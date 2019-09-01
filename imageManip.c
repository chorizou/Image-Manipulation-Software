//gwang46, czou9
//Image manipulation functions

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "imageManip.h"
#include "ppm_io.h"

#define M_PI 3.14159265358979323846


/* Takes an image and changes its color channel
 * First call turns image purple
 * Second call turns image green
 * Third call turns image orig
 */
Image * swap(Image * im){

  Image * new_im = malloc(sizeof(Image));
  new_im->data = malloc(sizeof(Pixel) * im->rows * im->cols);

  for (int i = 0; i < im->rows * im->cols; i++) {
    (new_im->data + i)->r = (im->data + i)->g;
    (new_im->data + i)->g = (im->data + i)->b;
    (new_im->data + i)->b = (im->data + i)->r;
  }

  new_im->rows = im->rows;
  new_im->cols = im->cols;
  
  return new_im;
}



/*
 *
 */
Image * bright(Image * raw, int intensity){

  assert(raw);

  Image * edit = malloc(sizeof(Image));

  /*
  ASSUMPTION
  Malloc works
  Like swap, unsure how to return errors
  if(!edit){
    fprintf(stderr, "Image allocation failed!\n");
    exit(1);
  }
  */

  
  edit->rows = raw->rows;
  edit->cols = raw->cols;
  
  Pixel * px = malloc(sizeof(Pixel) * edit->rows * edit->cols);

  edit->data = px;
  
  for(int i = 0; i < raw->rows; i++) {
    for(int j = 0; j < raw->cols; j++) {

      int temp[3];

      //Add intensity to each channel in the Pixel
      //Need to explicitly cast the unsigned to an int
      temp[0] = (int) raw->data[i * edit->cols + j].r + intensity;
      temp[1] = (int) raw->data[i * edit->cols + j].g + intensity;
      temp[2] = (int) raw->data[i * edit->cols + j].b + intensity;

      int k = clamp(temp);
      
      if(k != 0){
	fprintf(stderr, "Error clamping values");
	exit(1);
      }

      //Add altered values into the edited image struct
      //Need to cast values back to unsigned for the pixels to take them
      edit->data[i * edit->cols + j].r = (unsigned char) temp[0];
      edit->data[i * edit->cols + j].g = (unsigned char) temp[1];
      edit->data[i * edit->cols + j].b = (unsigned char) temp[2];
      
    }
  }

  return edit; 
  
}

int clamp(int test[]) {

  //Test if red value is between 0 and 255
  //Clamp to nearest number if necessary
  if(test[0] > 255 || test[1] > 255 || test[2] > 255) {
    test[0] = 255;
    test[1] = 255;
    test[2] = 255;
   }
  else if(test[0] < 0 || test[1] < 0 || test[2] < 0) {
    test[0] = 0;
    test[1] = 0;
    test[2] = 0;
  }
  /*
  //Test if blue value is between 0 and 255
  //Clamp to nearest number if necessary
  else if(test[1] > 255) {
    test[1] = 255;
  }
  else if(test[1] < 0) {
    test[1] = 0;
  }

  //Test if green value is between 0 and 255
  //Clamp to nearest number if necessary
  else if(test[2] > 255) {
    test[2] = 255;
  }
  else if(test[2] < 0) {
    test[2] = 0;
  }
  */

  //No errors encountered
  return 0;
}
  

Image * invert(Image * raw){

  Image * edit = malloc(sizeof(Image));
  /*
  if(!edit){
    fprintf(stderr, "Image allocation failed!\n");
    exit(1);
  }
  */
  
  edit->rows = raw->rows;
  edit->cols = raw->cols;

  Pixel * px = malloc(sizeof(Pixel) * edit->rows * edit->cols);
  
  edit->data = px; 
  
  for(int i = 0; i < raw->rows; i++) {
    for(int j = 0; j < raw->cols; j++) {
      edit->data[i * edit->cols + j].r = 255 - (raw->data[i * edit->cols + j].r);
      edit->data[i * edit->cols + j].g = 255 - (raw->data[i * edit->cols + j].g);
      edit->data[i * edit->cols + j].b = 255 - (raw->data[i * edit->cols + j].b);
    }
  }
  
  return edit;
 
}

Image * gray(Image * raw){

  
  Image * edit = malloc(sizeof(Image));

  edit->rows = raw->rows;
  edit->cols = raw->cols;

  Pixel * px = malloc(sizeof(Pixel) * edit->rows * edit->cols);
  edit->data = px;
  
  for(int i = 0; i < raw->rows; i++) {
    for(int j = 0; j < raw->cols; j++) {

      double intensity = 0.30 * (raw->data[i * edit->cols + j].r) + 0.59 * (raw->data[i * edit->cols + j].g) + 0.11 * (raw->data[i * edit->cols + j].b);
    
      edit->data[i * edit->cols + j].r = (unsigned) intensity;
      edit->data[i * edit->cols + j].g = (unsigned) intensity;
      edit->data[i * edit->cols + j].b = (unsigned) intensity;
    }
  }

  return edit; 
}



Image * crop(Image * raw, int left_col, int top_row, int right_col, int bot_row){

  if(raw->rows < bot_row || raw->cols < right_col || left_col < 0 || top_row < 0)
    exit(1);
  else if(top_row > bot_row || left_col > right_col)
    exit(1);
  
  int length = bot_row - top_row;
  int width = right_col - left_col;

  Image * edit = malloc(sizeof(Image));

  edit->rows = length;
  //printf("%d\n", edit->rows);
  
  edit->cols = width;
  //printf("%d\n", edit->cols);
  
  Pixel * px = malloc(sizeof(Pixel) * edit->rows * edit->cols);
  edit->data = px;
  
  /*
  if(!edit){
    fprintf(stderr, "Image allocation failed!\n");
    exit(1);
  }
  */
  
  for(int i = top_row; i < bot_row; i++) {
    for(int j = left_col; j < right_col; j++) {
      edit->data[(i - top_row) * edit->cols + (j - left_col)].r = raw->data[i * raw->cols + j].r;
      edit->data[(i - top_row) * edit->cols + (j - left_col)].g = raw->data[i * raw->cols + j].g;
      edit->data[(i - top_row) * edit->cols + (j - left_col)].b = raw->data[i * raw->cols + j].b;
    }
  }

  return edit;
    
}


/* Blurs the image to the extent specified by the user
 * @param im image to be blurred
 * @param sigma the extent to be blured to
 */
Image * blur(Image * im, double sigma ) {

  Image * new_im = malloc(sizeof(Image));
  new_im->rows = im->rows;
  new_im->cols = im->cols;
  new_im->data = malloc(sizeof(Pixel) * im->rows * im->cols);

  double ** gd = g_matrix(sigma);

  for (int r = 0; r < im->rows; r++) {
    for (int c = 0; c < im->cols; c++) {
      new_im->data[r * im->cols + c] = apply_filter(im, r, c, gd, sigma);
    }
  }

  int n = (int)(sigma * 10);
  if (n % 2 == 0)
    n += 1;
  
  for (int i = 0; i < n; i++)
    free(gd[i]);
  free(gd);

  return new_im;
}

/* square function
 */
double sq(double x) {
  x *= x;
  return x;
}

/* calculates guassian matrix
 * WORKS!
 */
double ** g_matrix(double sigma) {

  //printf("sigma: %f", sigma);
  int n = (int) (sigma * 10);
  if (n % 2 == 0)
    n += 1;
  //printf("n: %d\n", n);

  double ** gd = malloc(sizeof(double*) * n);
  for (int i = 0; i < n; i++)
    gd[i] = malloc(sizeof(double) * n);

  int counter = 0;
  for (int r = 0; r < n; r++) {
    int dx = r - n/2;
    for (int c = 0; c < n; c++) {
      int dy = c - n/2;
      //printf("dx: %d dy: %d\n", dx, dy);
      double g = (1.0 / (2.0 * M_PI * sq(sigma))) * exp(-(sq(dx) + sq(dy)) / (2 * sq(sigma)));
      gd[r][c] = g;
      counter++;
    }
  }

  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%f ", gd[i][j]);
    }
    printf("\n");
    }

  return gd;
}


// row: row of px
// col: col of px
// gd: gaussian matrix
// n: side length of guassian matrix
Pixel apply_filter(Image * im, int row, int col, double ** gd, double sigma) {

  int n = (int) (sigma * 10);
  if (n % 2 == 0)
    n += 1;

  // check that all four corners are in bounds
  int start_r = row - n/2;
  int start_c = col - n/2;
  int end_r = row + n/2;
  int end_c = col + n/2;

  int gd_r = 0;
  int gd_c = 0;
  int gd_r_end = n;
  int gd_c_end = n;

  if (start_r < 0) {
    gd_r = 0 - start_r;
    start_r = 0;
  }
  if (start_c < 0) {
    gd_c = 0 - start_c;
    start_c = 0;
  }
  if (end_r > im->rows) {
    gd_r_end = n - (end_r - im->rows);
    end_r = im->rows;
  }
  if (end_c > im->cols) {
    gd_c_end = n - (end_c - im->cols);
    end_c = im->cols;
  }

  //printf("gdr: %d gdend: %d gdc: %d gdend: %d", gd_r, gd_r_end, gd_c, gd_c_end);

  // calculate filter value
  double filter_val = 0;
  for (int i = gd_r; i < gd_r_end; i++) {
    for (int j = gd_c; j < gd_c_end; j++) {
      filter_val += gd[i][j];
      //printf("filter val: %f", filter_val);
    }
  }


  double im_val_r = 0;
  double im_val_g = 0;
  double im_val_b = 0;

  // loop thru valid areas within orginal image
  // and convert every color channel
  for (int r = start_r; r < end_r; r++) {
    for (int c = start_c; c < end_c; c++) {
      int cur = r * im->cols + c;
      
      im_val_r += (im->data[cur].r * gd[gd_r][gd_c]);
      im_val_g += (im->data[cur].g * gd[gd_r][gd_c]);
      im_val_b += (im->data[cur].b * gd[gd_r][gd_c]);

      if (gd_c + 1 >= n)
	break;
      gd_c++;
    }
    if (gd_r + 1 >= n)
      break;
    gd_r++;
  }

  //printf("im: %f fv: %f\n", im_val_r, filter_val);
  
  // normalize new single pixel
  Pixel new_p;
  new_p.r = im_val_r / filter_val;
  new_p.g = im_val_g / filter_val;
  new_p.b = im_val_b / filter_val;

  return new_p;
}



/*
 *
 */
Image * edges (Image * im, double sigma, int threshold) {

  Image * temp_im = gray(im);
  temp_im = blur(temp_im, sigma);

  double x_int[im->rows][im->cols];
  double y_int[im->rows][im->cols];
  double total[im->rows][im->cols];

  for (int r = 1; r < im->rows - 1; r++) {
    for (int c = 1; c < im->cols - 1; c++) {
      x_int[r][c] = x_intensity(r, c, temp_im);
      y_int[r][c] = y_intensity(r, c, temp_im);
    }
  }

  for (int r = 1; r < im->rows - 1; r++)
    for (int c = 1; c < im->cols - 1; c++)
      total[r][c] = sqrt(sq(x_int[r][c]) + sq(y_int[r][c]));


  Image * new_im = malloc(sizeof(Image));
  new_im->data = malloc(sizeof(Pixel) * new_im->rows * new_im->cols);
  
  for (int i = 1; i < new_im->rows - 1; i++) {
    for (int j = 1; j < new_im->cols - 1; j++) {

      int cur =  i * new_im->cols + j;

      if (total[i][j] > threshold) {
        new_im->data[cur].r = 0;
        new_im->data[cur].g = 0;
        new_im->data[cur].b = 0;
      } else {
	new_im->data[cur].r = 255;
        new_im->data[cur].g = 255;
        new_im->data[cur].b = 255;
      }
    }
  }

  return new_im;
}


// calculate intensity in the x direction
double x_intensity(int x, int y, Image * im) {

  double x1 = im->data[(x + 1) * im->cols + y].r;
  double x2 = im->data[(x - 1) * im->cols + y].r;
  double x_intensity = (x1 - x2) / 2;

  return x_intensity;
}


// calculate intensity in the y direction
double y_intensity(int x, int y, Image * im) {

  double y1 = im->data[x * im->cols + (y + 1)].r;
  double y2 = im->data[x * im->cols + (y - 1)].r;
  double y_intensity = (y1 - y2) / 2;

  return y_intensity;
}

