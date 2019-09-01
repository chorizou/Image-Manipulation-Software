// Temporary use to test files
// czou9

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ppm_io.h"

#define M_PI 3.14159265358979323846

double sq(double x);
double ** g_matrix(double sigma);
Pixel apply_filter(Image * im, int row, int col, double ** gd, double sigma);
Image * blur(Image * im, double sigma);


int main() {

  printf("Please enter a sigma value: ");
  double sigma;
  scanf(" %lf", &sigma);
  double ** filter_p = g_matrix(sigma);

  return 0;
}

// square function
double sq(double x) {
  x *= x;
  return x;
}


/* calculates guassian matrix
 * WORKS!
 */
double ** g_matrix(double sigma) {

  int n = (int) (sigma * 10);
  if (n % 2 == 0)
    n += 1;
  //printf("n: %d\n", n);

  double **gd = malloc(sizeof(double*) * n);
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

  /*
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%f ", gd[i][j]);
    }
    printf("\n");
    }*/

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

  // calculate filter value
  double filter_val;
  for (int i = gd_r; i < gd_r_end; i++)
    for (int j = gd_c; j < gd_c_end; j++)
      filter_val += gd[i][j];


  Pixel * px = im->data;
  //Pixel * new_px = malloc(sizeof(struct Pixel) * im->rows * im->cols);
  double im_val_r;
  double im_val_g;
  double im_val_b;
  
  // loop thru valid areas within orginal image
  // and convert every color channel
  for (int r = start_r; r < end_r; r++) {
    for (int c = start_c; c < end_c; c++) {
      Pixel p = *(px + r * im->cols * sizeof(int) + c * sizeof(int));   
      Pixel new_p;
      new_p.r = (p.r) * gd[gd_r][gd_c]; im_val_r += new_p.r;
      new_p.g = (p.g) * gd[gd_r][gd_c]; im_val_g += new_p.g;
      new_p.b = (p.b) * gd[gd_r][gd_c]; im_val_b += new_p.b;
      //*new_px = new_p;
      //*new_px += 1;
      gd_c++;
    }
    gd_r++;
  }

  // normalize new single pixel
  Pixel p = *(px + row * im->cols * sizeof(int) + col * sizeof(int));
  Pixel new_p;
  new_p.r = (p.r) * gd[n/2][n/2] * (im_val_r / filter_val);
  new_p.g = (p.g) * gd[n/2][n/2] * (im_val_g / filter_val);
  new_p.b = (p.b) * gd[n/2][n/2] * (im_val_b / filter_val);

  return new_p;
}

// Loop thru entire image array and overlay filter
Image * blur(Image * im, double sigma) {

  Image * new_im = malloc(sizeof(Image));
  Pixel * px = malloc(sizeof(Pixel) * im->rows * im->cols);

  double ** gd = g_matrix(sigma);
  
  // applying filter to every pixel and storing new pixels in new px array
  int counter = 0;
  for (int r = 0; r < im->rows; r++) {
    for (int c = 0; c < im->cols; c++) {
      Pixel new_p = apply_filter(im, r, c, gd, sigma);
      *(px + counter) = new_p;
      counter++;
    }
  }

  new_im->data = px;

  /*
  for (int i = 0; i < n; i++)
    free(gd[i]);
  free(gd);
  free(px);
  */

  return new_im;
}
