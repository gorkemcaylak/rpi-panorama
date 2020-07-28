#include <cmath>
#include "image.h"
#include <iostream>


using namespace std;

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the nearest neibor to pixel (x,y,c)
float Image::pixel_nearest(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)

  // int X = (int)(x);
  // int Y = (int)(y);
  int X = (int)floorf(x);
  int Y = (int)floorf(y);
  return clamped_pixel(X, Y, c);

  }

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the bilinearly interpolated pixel (x,y,c)
float Image::pixel_bilinear(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)

  float X0 = floor(x+0.5) - 0.5;
  float X1 = X0 + 1;
  float weightX = x - X0;

  float Y0 = floor(y+0.5) - 0.5;
  float Y1 = Y0 + 1;
  float weightY = y - Y0;

  X0 -= 0.5;
  X1 -= 0.5;
  Y0 -= 0.5;
  Y1 -= 0.5;

  float val = weightX       * weightY       * clamped_pixel(X1, Y1, c)
            + (1.0-weightX) * weightY       * clamped_pixel(X0, Y1, c)
            + weightX       * (1.0-weightY) * clamped_pixel(X1, Y0, c)
            + (1.0-weightX) * (1.0-weightY) * clamped_pixel(X0, Y0, c);
  
  return val;
  }

// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image nearest_resize(const Image& im, int w, int h)
  {
  // Image ret(w,h,im.c);
  
  // float w_ratio = ((float)w)/((float)im.w);
  // float h_ratio = ((float)h)/((float)im.h);

  // for(int i=0; i<w; i++){
  //   for(int j=0; j<h; j++){
  //     for(int k=0; k<3; k++){
  //       ret(i, j, k) = im.pixel_nearest(((float)i+0.5)/w_ratio, ((float)j+0.5)/h_ratio, k);        
  //     }
  //   }
  // }
  Image ret(w,h,im.c);
  int c = im.c;
  float w_ratio = ((float)w)/((float)im.w);
  float h_ratio = ((float)h)/((float)im.h);
  float val;
  for(int i=0; i<w; i++){
    for(int j=0; j<h; j++){
      for(int k=0; k<c; k++){
        float x_ = ((float)i+0.5)/w_ratio; //(float) necessary?
        float y_ = ((float)j+0.5)/h_ratio;
        val = im.pixel_nearest(x_, y_, k);   
        ret.set_pixel(i, j, k, val);     
      }
    }
  }

  return ret;
  }


// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image bilinear_resize(const Image& im, int w, int h)
  {
  Image ret(w,h,im.c);
  int c = im.c;
  float w_ratio = ((float)w)/((float)im.w);
  float h_ratio = ((float)h)/((float)im.h);
  for(int i=0; i<w; i++){
    for(int j=0; j<h; j++){
      for(int k=0; k<c; k++){
        ret(i, j, k) = im.pixel_bilinear(((float)i+0.5)/w_ratio, ((float)j+0.5)/h_ratio, k); 
      }
    }
  }
  return ret;
  }