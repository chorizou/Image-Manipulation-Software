// gwang46, czou9
// ppm_io.c
// 601.220, Spring 2019
// Starter code for midterm project

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ppm_io.h"
#include <string.h>

int isdigit(int arg);


/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  // check assumption that fp is not NULL
  assert(fp);

  char tag[3];
  fgets(tag, sizeof(tag), fp);
  //printf("%s\n", tag);
  if (strcmp(tag, "P6") != 0) {
    fprintf(stderr, "File not in ppm format!\n");
    exit(3);
  }

  // skip the comment line  
  //Ignore everything except for lines that start with numbers
  char buf[1024];
  while(fgets(buf, sizeof(buf), fp) != NULL){
     
    if (isdigit(buf[0]) > 0 ) {
      int offset = -(strlen(buf) * sizeof(char));
      fseek(fp, offset, SEEK_CUR);
      break;
    }
  }

  //scan in number of columns, rows, and color
  //checks to see if they are valid
  int cols, rows, color;
  int h = fscanf(fp, " %d", &cols);
  int j = fscanf(fp, " %d", &rows);
  int k = fscanf(fp, " %d ", &color);
  //printf("c: %d r: %d col: %d\n", cols, rows, color);
  int i = h + j + k;
  
  //printf("i: %d col: %d row: %d color: %d", i, cols, rows, color);
  if (i != 3) {
    fprintf(stderr, "File not in ppm format!\n");
    exit(3);
  }
  if (cols <= 0 || rows <= 0 || color != 255) {
    fprintf(stderr, "Ppm format error!\n");
    exit(3);
  }
  /*****************************WORKS ^^ ************************************/
  
  // allocate space for an image
  Image * im = malloc(sizeof(Image));
  if (!im) {
    fprintf(stderr, "Image allocation failed\n");
    exit(1);
  }

  im->rows = rows;
  im->cols = cols;

  // allocate space for an array of Pixels
  Pixel * px = malloc(sizeof(Pixel) * rows * cols);
  if (!px) {
    fprintf(stderr, "Pixel array allocation failed\n");
    free(im);
    exit(1);
  }  

  im->data = px;

  
  // scan in the pixel values
  int num_pixels_read = fread(px, sizeof(Pixel), im->cols * im->rows, fp);
  if (num_pixels_read != im->rows * im->cols) {
    fprintf(stderr, "Pixel data failed to read correctly\n");
    exit(1);
  }
  
  return im;
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // check that fp is not NULL
  assert(fp);
  assert(im);

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->cols * im->rows, fp);

  if (num_pixels_written != im->cols * im->rows) {
    fprintf(stderr, "Pixel data failed to write properly\n");
  }

  return num_pixels_written;
}

