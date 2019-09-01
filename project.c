//gwang46, czou9
//main() function

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ppm_io.h"
#include "imageManip.h"

int main(int argc, char ** argv) {

  printf("%d\n", argc);
  /*
  // test read_ and write_ppm
  char * input_file = argv[1];
  char * output_file = argv[2];
  FILE * input = fopen(input_file, "r");
  FILE * output = fopen(output_file, "w");
  Image * temp = read_ppm(input);
  write_ppm(output, temp);
  */

  
  // check for input, output, and operation
  if (argc < 3) {
    printf("Error: must include input/output filename!\n");
    return 1;
  }
  if (argc < 4) {
    printf("Error: no operation name specified!\n");
    return 4;
  }

  // check input/output for valildity
  char *input_file = argv[1];
  char *output_file = argv[2];
  
  if (input_file == NULL) {
    printf("Error: could not find input file\n");
    return 1;
  }
  if (output_file == NULL) {
    printf("Error: could not find output file\n");
    return 1;
  }

  FILE * input = fopen(input_file, "r");
  FILE * output = fopen(output_file, "w");

  if (input == NULL) {
    printf("Error: could not open input file!\n");
    return 2;
  }
  if (output == NULL) {
    printf("Error: could not open output file!\n");
    return 7;
  }

  // convert input file to image format
  Image * im = read_ppm(input);

  // check operation validity
  // if it's valid, cont
  char * operation = argv[3];
  if (strcmp(operation, "swap") == 0) {
    if (argc > 4) {
      printf("Illegal number of arguments for specified operation!\n");
      return 5;
    }
    Image * new_im = swap(im);
    write_ppm(output, new_im);
    
    free(new_im->data);
    free(new_im);
    
  } else if (strcmp(operation, "bright") == 0) {
    if (argc > 5) {
      printf("Illegal number of arguments for specified operation!\n");
      return 5;
    }
    int intensity = atoi(argv[4]);
    Image * new_im = bright(im, intensity);
    write_ppm(output, new_im);
    
    free(new_im->data);
    free(new_im);
    
  } else if (strcmp(operation, "invert") == 0) {
    if (argc > 4) {
      printf("Illegal number of arguments for specified operation!\n");
      return 5;
    }
    Image * new_im = invert(im);
    write_ppm(output, new_im);
    
  } else if (strcmp(operation, "gray") == 0) {
    if (argc > 4) {
      printf("Illegal number of arguments for specified operation!\n");
      return 5;
    }
    Image * new_im = gray(im);
    write_ppm(output, new_im);

    free(new_im->data);
    free(new_im);

  } else if (strcmp(operation, "crop") == 0) {
    if (argc > 8) {
      printf("Illegal number of arguments for specified operation!\n");
      return 5;
    }
    int a = atoi(argv[4]);
    int b = atoi(argv[5]);
    int c = atoi(argv[6]);
    int d = atoi(argv[7]);

    Image * new_im = crop(im, a, b, c, d);
    write_ppm(output, new_im);

    free(new_im->data);
    free(new_im);
    
  } else if (strcmp(operation, "blur") == 0) {
    if (argc != 5) {
      printf("Illegal number of arguments for specified operation!\n");
      return 5;
    }
    double sigma = atof(argv[4]);
    //printf("main sig: %f", sigma);
    //printf("str: %s", argv[4]);

    Image * new_im = blur(im, sigma);
    write_ppm(output, new_im);

    free(new_im->data);
    free(new_im);
    
  } else if (strcmp(operation, "edges") == 0) {
    if (argc != 6) {
      printf("Illegal number of arguments for specified operation!\n");
      return 5;
    }
    double sigma = atof(argv[4]);
    int threshold = atoi(argv[5]);

    Image * new_im = edges(im, sigma, threshold);
    write_ppm(output, new_im);

    free(new_im->data);
    free(new_im);
  }

  free(im->data);
  free(im);


  // check if input and output have errors
  if (ferror(input)) {
    printf("Error: error indicator was set for input file\n");
    return 2;
  }
  if (ferror(output)) {
    printf("Error: error indicator was est for output file\n");
    return 7;
  }
  
  fclose(input);
  fclose(output);
  
  //*/
  return 0;
}



