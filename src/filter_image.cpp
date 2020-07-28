#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <iostream>
#include "image.h"

#define M_PI 3.14159265358979323846

// HW1 #2.1
// Image& im: image to L1-normalize
void l1_normalize(Image& im)
  {
  float sum;
  // Normalize each channel
  for(int k=0; k<im.c; k++){  
    sum = 0;
    for(int i=0; i<im.w; i++){
      for(int j=0; j<im.h; j++){
        sum += im(i,j,k);
      }
    }
    for(int i=0; i<im.w; i++){
      for(int j=0; j<im.h; j++){
        im(i,j,k) /= sum;
      }
    }
  }
}

// HW1 #2.1
// int w: size of filter
// returns the filter Image of size WxW
Image make_box_filter(int w)
  {
  assert(w%2); // w needs to be odd
  Image box(w, w, 1);
  for(int i=0; i<w; i++){
    for(int j=0; j<w; j++){
      box(i,j,0) = 1.0;
    }
  }
  l1_normalize(box);
  return box;
  }

// HW1 #2.2
// const Image&im: input image
// const Image& filter: filter to convolve with
// bool preserve: whether to preserve number of ch
// returns the convolved image
Image convolve_image(const Image& im, const Image& filter, bool preserve)
  {
  // assert(filter.c==1);
  // Image ret = Image(im.w, im.h, im.c);
  // Image single(im.w, im.h, 1); 
  // // This is the case when we need to use the function clamped_pixel(x,y,c).
  // // Otherwise you'll have to manually check whether the filter goes out of bounds
  // float sum;

  // for(int k=0; k<im.c; k++){  
  //     for(int i=0; i<im.w; i++){
  //       for(int j=0; j<im.h; j++){
  //         sum = 0.0;
  //         for(int s=0; s<filter.w; s++){
  //           for(int m=0; m<filter.h; m++){
  //             sum += im.clamped_pixel(i-filter.w/2+s, j-filter.h/2+m, k) * filter(s,m);
  //           }
  //         }
  //         ret(i, j, k) = sum;
  //       }
  //     }
  //   }
  // if(!preserve){
  //   for(int i=0; i<ret.w; i++){
  //     for(int j=0; j<ret.h; j++){
  //       single(i, j, 0) = ret(i, j, 0) + ret(i, j, 1) + ret(i, j, 2);
  //     }
  //   }
  //   return single;
  // }

  // return ret;
    assert(filter.c==1);
     int offsetW = (int)floorf((float)filter.w / 2);
     int offsetH = (int)floorf((float)filter.h / 2);
     int C;
     if (preserve == 1) {
       C = im.c;
     } else {
       C = 1;
     }

     Image ret(im.w, im.h, C);
     for (int c = 0; c < im.c; c++) {
       for (int h = 0; h < im.h; h++) {
         for (int w = 0; w < im.w; w++) {
           for (int y = 0; y < filter.h; y++) {
             for (int x = 0; x < filter.w; x++) {
               float weight = filter.clamped_pixel(x, y, (filter.c == 1) ? 0 : c);

               float value = im.clamped_pixel(w + x - offsetW, h + y - offsetH, c); // this one is exceeding the boundaries!


               float origin_value = ret.clamped_pixel(w, h, (preserve == 1) ? c : 0);
               ret.set_pixel(w, h, (preserve == 1) ? c : 0, origin_value + weight * value);
             }
           }
         }
       }

     }
     return ret;
    
  }

// HW1 #2.3
// returns basic 3x3 high-pass filter
Image make_highpass_filter()
  {
   
  Image box(3, 3, 1);
  
  box(0,0,0) = 0;
  box(1,0,0) = -1;
  box(2,0,0) = 0;
  box(0,1,0) = -1;
  box(1,1,0) = 4;
  box(2,1,0) = -1;
  box(0,2,0) = 0;
  box(1,2,0) = -1;
  box(2,2,0) = 0;

  return box;
  
  }

// HW1 #2.3
// returns basic 3x3 sharpen filter
Image make_sharpen_filter()
  {
  Image box(3, 3, 1);
  
  box(0,0,0) = 0;
  box(1,0,0) = -1;
  box(2,0,0) = 0;
  box(0,1,0) = -1;
  box(1,1,0) = 5;
  box(2,1,0) = -1;
  box(0,2,0) = 0;
  box(1,2,0) = -1;
  box(2,2,0) = 0;

  return box;
  
  }

// HW1 #2.3
// returns basic 3x3 emboss filter
Image make_emboss_filter()
  {
  Image box(3, 3, 1);
  
  box(0,0,0) = -2;
  box(1,0,0) = -1;
  box(2,0,0) = 0;
  box(0,1,0) = -1;
  box(1,1,0) = 1;
  box(2,1,0) = 1;
  box(0,2,0) = 0;
  box(1,2,0) = 1;
  box(2,2,0) = 2;

  return box;
  
  }

// HW1 #2.4
// float sigma: sigma for the gaussian filter
// returns basic gaussian filter
Image make_gaussian_filter(float sigma)
  {
  int w = (int)ceilf(sigma * 6.0);
  if(w % 2 == 0) w++;
  Image gaus(w, w, 1);
  float ssq = sigma * sigma;
  float val = 1.0 / 2 / M_PI / ssq;
  int center = w/2;
  for(int i=0; i<w; i++){
    for(int j=0; j<w; j++){
      gaus(i, j) = val * exp(- (pow((center - i),2) + pow((center - j),2)) / 2 / ssq); 
    }
  }
  l1_normalize(gaus);
  return gaus;
  
  }


// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their sum
Image add_image(const Image& a, const Image& b)
  {
  assert(a.w==b.w && a.h==b.h && a.c==b.c); // assure images are the same size
  
  int w = a.w;
  int h = a.h;
  int c = a.c;

  Image res(w, h, c);
  for(int k=0; k<c; k++){  
    for(int i=0; i<w; i++){
      for(int j=0; j<h; j++){
        res(i, j, k) = a(i, j, k) + b(i, j, k);
      }
    }
  }
  
  return res;
  
  }

// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their difference res=a-b
Image sub_image(const Image& a, const Image& b)
  {
  assert(a.w==b.w && a.h==b.h && a.c==b.c); // assure images are the same size
  
  int w = a.w;
  int h = a.h;
  int c = a.c;

  Image res(w, h, c);
  for(int k=0; k<c; k++){  
    for(int i=0; i<w; i++){
      for(int j=0; j<h; j++){
        res(i, j, k) = a(i, j, k) - b(i, j, k);
      }
    }
  }
  
  return res;
  
  }

// HW1 #4.1
// returns basic GX filter
Image make_gx_filter()
  {
  Image box(3, 3, 1);
  
  box(0,0,0) = -1;
  box(1,0,0) = 0;
  box(2,0,0) = 1;
  box(0,1,0) = -2;
  box(1,1,0) = 0;
  box(2,1,0) = 2;
  box(0,2,0) = -1;
  box(1,2,0) = 0;
  box(2,2,0) = 1;

  return box;
  }

// HW1 #4.1
// returns basic GY filter
Image make_gy_filter()
  {
  Image box(3, 3, 1);
  
  box(0,0,0) = -1;
  box(1,0,0) = -2;
  box(2,0,0) = -1;
  box(0,1,0) = 0;
  box(1,1,0) = 0;
  box(2,1,0) = 0;
  box(0,2,0) = 1;
  box(1,2,0) = 2;
  box(2,2,0) = 1;

  return box;
  }

// HW1 #4.2
// Image& im: input image
void feature_normalize(Image& im)
  {
  assert(im.w*im.h); // assure we have non-empty image
  float min[3];
  float max[3];
  float temp_min, temp_max;
  for(int k=0; k<im.c; k++){  
    temp_min = im(0, 0, k);
    temp_max = im(0, 0, k);
    for(int i=0; i<im.w; i++){
      for(int j=0; j<im.h; j++){
        if(im(i, j, k) < temp_min) temp_min = im(i, j, k);
        else if(im(i, j, k) > temp_max) temp_max = im(i, j, k); //make if?
      }
    }
    min[k] = temp_min;
    max[k] = temp_max;
  }
  
  float range;
  for(int k=0; k<im.c; k++){  

    range = max[k] - min[k];
    
    if(range != 0){
      
      for(int i=0; i<im.w; i++){
        for(int j=0; j<im.h; j++){
          im(i, j, k) = (im(i, j, k) - min[k])/range;
        }
      }

    }
    else {
     
      for(int i=0; i<im.w; i++){
        for(int j=0; j<im.h; j++){
          im(i, j, k) = 0;
        }
      }
    
    }
  }

}


// Normalizes features across all ch
void feature_normalize_total(Image& im)
  {
  assert(im.w*im.h*im.c); // assure we have non-empty image
  
  int nc=im.c;
  im.c=1;im.w*=nc;
  
  feature_normalize(im);
  
  im.w/=nc;im.c=nc;
  
  }


// HW1 #4.3
// Image& im: input image
// return a pair of images of the same size
pair<Image,Image> sobel_image(const Image& im)
  {

  Image mag(im.w, im.h, 1);
  Image dir(im.w, im.h, 1);
  Image gxfilter = make_gx_filter();
  Image gyfilter = make_gy_filter();
  Image Gx(im.w, im.h, 1);
  Image Gy(im.w, im.h, 1);

  Gx = convolve_image(im, gxfilter, false);
  Gy = convolve_image(im, gyfilter, false);
  // Gx.save_png("output/Gx");
  // Gy.save_png("output/Gy");
  // cout<<"sobel prog1"<<endl;

  for(int i=0; i<im.w; i++){
    for(int j=0; j<im.h; j++){
      mag(i, j) = sqrt(pow(Gx(i, j), 2) + pow(Gy(i, j), 2));
      dir(i, j) = atan2(Gy(i, j), Gx(i, j));
    }
  }
  // cout<<"sobel prog1"<<endl;

  return {mag,dir};
  }


// HW1 #4.4
// const Image& im: input image
// returns the colorized Sobel image of the same size
Image colorize_sobel(const Image& im)
  {
  Image gaus = make_gaussian_filter(4);
  Image gaussed = convolve_image(im, gaus, true);

  pair<Image,Image> mypair = sobel_image(gaussed);
  Image mag = mypair.first;
  Image dir = mypair.second;

  Image result(im.w, im.h, 3);

  feature_normalize(mag);
  
  for(int i=0; i<im.w; i++){
    for(int j=0; j<im.h; j++){
      result(i, j, 0) = dir(i, j) / 2 / M_PI + 0.5;
      result(i, j, 1) = mag(i, j);
      result(i, j, 2) = mag(i, j);
    }
  }
  
  hsv_to_rgb(result);

  
  return result;
  }


// HW1 #4.5
// const Image& im: input image
// float sigma1,sigma2: the two sigmas for bilateral filter
// returns the result of applying bilateral filtering to im
Image bilateral_filter(const Image& im, float sigma1, float sigma2)
  {
  Image res = Image(im.w, im.h, im.c);
  float prev = 0;
  int dim = 6 * sigma1;
  dim = (dim %2 == 0) ? dim+1 : dim;
  for (int ch = 0; ch < im.c; ch++) {
    for (int row = 0; row < im.h; row++) {
      for (int col = 0; col < im.w; col++) {
        float normalized_factor = 0;
        float val = 0;
        for (int j = 0; j < dim; j++) {
            for (int i = 0; i < dim; i++) {
                int dx = col + (i - (dim/2.0)) + 0.5;
                int dy = row + (j - (dim/2.0)) + 0.5;
                float spat_gauss = expf(-((powf(dx - col, 2) + powf(dy - row, 2))/(2 * sigma1 * sigma1))) / (2 * M_PI * sigma1 * sigma1);
                float color_dist_gauss = expf(-(powf((im.clamped_pixel(dx, dy, ch) - im.clamped_pixel(col, row, ch)), 2))/(2 * sigma2 * sigma2)) / (2 * M_PI * sigma2 * sigma2);
                prev = color_dist_gauss;
                val += (spat_gauss * color_dist_gauss * im.clamped_pixel(dx, dy, ch));
                normalized_factor += (spat_gauss * color_dist_gauss);
              }
        }
        val /= normalized_factor;
        res(col, row, ch) = val;
      }
    }
  }
  return res;
  }



// HELPER MEMBER FXNS

void Image::feature_normalize(void) { ::feature_normalize(*this); }
void Image::feature_normalize_total(void) { ::feature_normalize_total(*this); }
void Image::l1_normalize(void) { ::l1_normalize(*this); }

Image operator-(const Image& a, const Image& b) { return sub_image(a,b); }
Image operator+(const Image& a, const Image& b) { return add_image(a,b); }
