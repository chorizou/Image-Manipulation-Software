// Temp file for edge function
// czou9


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ppm_io.h"
#include "temp.c"
#include "imageManip.c"

Image * grayscale(Image * im);
Image * blur(Image * im, double sigma);
double x_intensity(int x, int y, Image * im);
double y_intensity(int x, int y, Image * im);


// Edge function
Image * edge (Image * im, double sigma, int threshold) {

  Image * new_im = grayscale(im);
  new_im = blur(new_im, sigma);

  double x_int[im->rows][im->cols];
  double y_int[im->rows][im->cols];
  double total[im->rows][im->cols];

  for (int r = 1; r < im->rows - 1; r++) {
    for (int c = 1; c < im->cols - 1; c++) {
      x_int[r][c] = x_intensity(r, c, new_im);
      y_int[r][c] = y_intensity(r, c, new_im);
    }
  }

  for (int r = 1; r < im->rows - 1; r++)
    for (int c = 1; c < im->cols - 1; c++)
      total[r][c] = sqrt(sq(x_int[r][c]) + sq(y_int[r][c]));  

  Pixel * px = malloc(sizeof(Pixel) * new_im->rows * new_im->cols);
  for (int i = 1; i < new_im->rows - 1; i++) {
    for (int j = 1; j < new_im->cols - 1; j++) {
      int cur ind =  i * new_im->cols + j;
      
      if (total[i][j] > threshold) {
	*(px + cur * sizeof(Pixel))->r = 0;
	*(px + cur * sizeof(Pixel))->g = 0;
	*(px + cur * sizeof(Pixel))->b = 0;
      } else {
	*(px + cur * sizeof(Pixel))->r = 255;
	*(px + cur * sizeof(Pixel))->g = 255;
	*(px + cur * sizeof(Pixel))->b = 255;
      }
    }
  }

  new_im->data = px;
  free(px);

  return new_im;
}


// calculate intensity in the x direction
double x_intensity(int x, int y, Image * im) {

  Pixel * px = im->data;
  
  double x1 = px[(x + 1) * im->cols * sizeof(Pixel) + y * sizeof(Pixel)]->g;
  double x2 = px[(x - 1) * im->cols * sizeof(Pixel) + y * sizeof(Pixel)]->g;
  double x_intensity = (x1 - x2) / 2.0;

  return x_intensity;
}


// calculate intensity in the y direction
double y_intensity(int x, int y, Image * im) {

  Pixel * px = im->data;
  
  double y1 = px[x * im->cols * sizeof(Pixel) + (y + 1) * sizeof(Pixel)]->g;
  double y2 = px[(x - 1) * im->cols * sizeof(Pixel) + (y - 1) * sizeof(Pixel)]->g;
  double y_intensity = (y1 - y2) / 2.0;

  return y_intensity;
}

