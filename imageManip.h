//gwang46, czou
//imageManip.c header

#include <stdio.h>
#include <stdlib.h>
#include "ppm_io.h"


Image * swap(Image * raw);

Image * bright(Image * raw, int intensity);

int clamp(int test[]);

Image * invert(Image * raw);

Image * gray(Image * raw);

Image * crop(Image * raw, int left_col, int top_row, int right_col, int bot_row);

Image * blur(Image * im, double sigma);

Image * edges(Image * im, double sigma, int threshold);

double ** g_matrix(double sigma);

Pixel apply_filter(Image * im, int row, int col, double ** gd, double sigma);

double x_intensity(int x, int y, Image * im);

double y_intensity(int x, int y, Image * im);

double sq(double x);

//double sqrt(double x);

//double exp(double x);

