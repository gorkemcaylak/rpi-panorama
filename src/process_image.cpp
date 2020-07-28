#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>

#include <iostream>

#include "image.h"

using namespace std;

// HW0 #3
// const Image& im: input image
// return the corresponding grayscale image
Image rgb_to_grayscale(const Image& im)
  {
  assert(im.c == 3); // only accept RGB images
  Image gray(im.w,im.h,1); // create a new grayscale image (note: 1 channel)
  for(int i=0; i<im.w*im.h; i++){
    gray.data[i] = 0.299 * im.data[i] + 0.587 * im.data[i+im.w*im.h] + 0.114 * im.data[i+2*im.w*im.h]; 
  }  
  return gray;
  }



// Example function that changes the color of a grayscale image
Image grayscale_to_rgb(const Image& im, float r, float g, float b)
  {
  assert(im.c == 1);
  Image rgb(im.w,im.h,3);
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    rgb(q1,q2,0)=r*im(q1,q2);
    rgb(q1,q2,1)=g*im(q1,q2);
    rgb(q1,q2,2)=b*im(q1,q2);
    }
  
  return rgb;
  }




// HW0 #4
// Image& im: input image to be modified in-place
// int c: which channel to shift
// float v: how much to shift
void shift_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel

  if(c>=0 && c<im.c){
    for(int i=(c)*im.w*im.h; i<(c+1)*im.w*im.h; i++){
      im.data[i] = im.data[i] + v;
    }  
  }


  return;    
  }

// HW0 #8
// Image& im: input image to be modified in-place
// int c: which channel to scale
// float v: how much to scale
void scale_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: scale all the pixels at the specified channel
  if(c>=0 && c<im.c){
    for(int i=(c)*im.w*im.h; i<(c+1)*im.w*im.h; i++){
      im.data[i] = im.data[i] * v;
    }  
  }  
  }


// HW0 #5
// Image& im: input image to be modified in-place
void clamp_image(Image& im)
  {
  // TODO: clamp all the pixels in all channel to be between 0 and 1
  // for(int i=0; i<3*im.w*im.h; i++){
  //     im.data[i] = im.data[i] > 1.0 ? 1.0 : im.data[i] < 0.0 ? 0.0 : im.data[i];
  //   }  
  for(int k=0; k<im.c; k++){
    for(int i=0; i<im.w; i++){
      for(int j=0; j<im.h; j++){
        float get = im(i, j, k);
        im(i, j, k) = get > 1.0 ? 1.0 : get < 0.0 ? 0.0 : get;
       }
      } 
    }
  }

// These might be handy
float max(float a, float b, float c)
  {
  return max({a,b,c});
  }

float min(float a, float b, float c)
  {
  return min({a,b,c});
  }


// HW0 #6
// Image& im: input image to be modified in-place
void rgb_to_hsv(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  float r, g, b;
  float c;
  float v, h, s;
  
  for(int i=0; i<im.w; i++){
    for(int j=0; j<im.h; j++){
      r = im(i, j, 0);
      g = im(i, j, 1);
      b = im(i, j, 2);
      v = max(r, g, b);
      c = (v - min(r, g, b));
      s = v > 0.0 ? (c / v) : 0.0;
      if(c == 0.0){
        h = 0;
      }
      else{
        if(v == r)
            h = (g-b)/c;
        else if(v == g)
            h = (b-r)/c + 2;
        else if(v == b)
            h = (r-g)/c + 4;
        
        if(h < 0.0)
          h = h/6 + 1;
        else
          h = h/6;
      }
      im(i, j, 0) = h;
      im(i, j, 1) = s;
      im(i, j, 2) = v;
    }
  }
  
  }

// HW0 #7
// Image& im: input image to be modified in-place
void hsv_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
    
  float v, h, s;
  float c, x, m;
  float r, g, b;

  // TODO: Convert all pixels from HSV format to RGB format
  for(int i=0; i<im.w; i++){
    for(int j=0; j<im.h; j++){
      h = im(i, j, 0);
      s = im(i, j, 1);
      v = im(i, j, 2);
      
      c = v*s;
      x = c*(1 - fabs((fmod(6 * h,2)) - 1));
      m = v-c;

      r = m;
      g = m;
      b = m;

      if(0.0 <= h && h < 1.0/6.0){
        r += c;
        g += x;
      }
      else if(1.0/6.0 <= h && h < 2.0/6.0){
        r += x;
        g += c;
      }
      else if(2.0/6.0 <= h && h < 3.0/6.0){
        g += c;
        b += x;
      }
      else if(3.0/6.0 <= h && h < 4.0/6.0){
        g += x;
        b += c;
      }
      else if(4.0/6.0 <= h && h < 5.0/6.0){
        r += x;
        b += c;
      }
      else if(5.0/6.0 <= h && h < 1.0){
        r += c;
        b += x;
      }

      im(i, j, 0) = r;
      im(i, j, 1) = g;
      im(i, j, 2) = b;
    }
  }
  
  }

// HW0 #9
// Image& im: input image to be modified in-place

void rgb_to_lrgb(Image& im);
void lrgb_to_xyz(Image& im);
void xyz_to_luv(Image& im);
void luv_to_lch(Image& im);

void rgb_to_lch(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to LCH format
  rgb_to_lrgb(im);
  lrgb_to_xyz(im);
  xyz_to_luv(im);
  luv_to_lch(im);
  
  }

// HW0 #9
// Image& im: input image to be modified in-place
void lch_to_luv(Image& im);
void luv_to_xyz(Image& im);
void xyz_to_rgb(Image& im);

void lch_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from LCH format to RGB format
  lch_to_luv(im);
  luv_to_xyz(im);
  xyz_to_rgb(im);
  
  }

//created for testing purpose only
void rgb_to_xyz(Image& im)
{
  //PASS
  rgb_to_lrgb(im);
  lrgb_to_xyz(im);
}

void rgb_to_lrgb(Image& im)
  {
    float V,v;
    for(int i=0; i<im.w; i++){
      for(int j=0; j<im.h; j++){
        for(int c=0; c<im.c; c++){
        V = im(i, j, c);  
        v = (V<=0.04045) ? V/12.92 : pow((V+0.055)/1.055, 2.4);
        im(i, j, c) = v;
        }
      }
    }

  }

  void lrgb_to_xyz(Image& im)
  {
    float r,g,b,X,Y,Z;
    float CIE_M[3][3] = {{0.4887180,  0.3106803,  0.2006017},{0.1762044,  0.8129847,  0.0108109},{0.0000000,  0.0102048,  0.9897952}};
    for(int i=0; i<im.w; i++){
      for(int j=0; j<im.h; j++){
        
        r = im(i, j, 0);
        g = im(i, j, 1);
        b = im(i, j, 2);

        X = CIE_M[0][0]*r + CIE_M[0][1]*g + CIE_M[0][2]*b;
        Y = CIE_M[1][0]*r + CIE_M[1][1]*g + CIE_M[1][2]*b;
        Z = CIE_M[2][0]*r + CIE_M[2][1]*g + CIE_M[2][2]*b;

        im(i, j, 0) = X;
        im(i, j, 1) = Y;
        im(i, j, 2) = Z;
        
      }
    }

  }

  void xyz_to_luv(Image& im)
  {
    float X,Y,Z, L,u,v;
    float Xr, Yr, Zr;
    float yr;
    float u1, v1, ur, vr, e, k;

    Xr = 0.9807;
    Yr = 1.0000;
    Zr = 1.1822;

    e = 216.0/24389.0;
    k = 24389.0/27.0;

    for(int i=0; i<im.w; i++){
      for(int j=0; j<im.h; j++){
        
        X = im(i, j, 0);
        Y = im(i, j, 1);
        Z = im(i, j, 2);

        yr = Y/Yr;
        u1 = 4*X / (X+15*Y+3*Z);
        v1 = 9*Y / (X+15*Y+3*Z);
        ur = 4*Xr/ (Xr+15*Yr+3*Zr);
        vr = 9*Yr/ (Xr+15*Yr+3*Zr);

        L = (yr > e) ? 116*pow(yr,1.0/3.0)-16 : k*yr;
        u = 13 * L * (u1 - ur);
        v = 13 * L * (v1 - vr);
        
        im(i, j, 0) = L;
        im(i, j, 1) = u;
        im(i, j, 2) = v;
        
      }
    }

  }

  void luv_to_lch(Image& im)
  {
    float L,U,V;
    float c,h;
    float atanVU;

    for(int i=0; i<im.w; i++){
      for(int j=0; j<im.h; j++){
        
        L = im(i, j, 0);
        U = im(i, j, 1);
        V = im(i, j, 2);

        atanVU = atan (V/U) * 180 / M_PI;

        //L = L;
        c = sqrtf(U*U + V*V);
        h = (atanVU >= 0) ? atanVU : (atanVU + 360);
        im(i, j, 0) = L;
        im(i, j, 1) = c;
        im(i, j, 2) = h;
        
      }
    }

  }

  void lch_to_luv(Image& im)
  {
    
    float L,U,V;
    float c,h;

    for(int i=0; i<im.w; i++){
      for(int j=0; j<im.h; j++){
        
        L = im(i, j, 0);
        c = im(i, j, 1);
        h = im(i, j, 2);

        //L = L;
        U = c*cos(h); 
        V = c*sin(h);

        im(i, j, 0) = L;
        im(i, j, 1) = U;
        im(i, j, 2) = V;
        
      }
    }


  }
void luv_to_xyz(Image& im)
{
  float Xr, Yr, Zr;
  float e, k;
  float a, b, c, d;
  float u0, v0;
  float L, U, V;
  float X, Y, Z;

  Xr = 0.9807;
  Yr = 1.0000;
  Zr = 1.1822;
  
  e = 0.008856;
  k = 903.3;

  u0 = 4*Xr/(Xr+15*Yr+3*Zr);
  v0 = 9*Yr/(Xr+15*Yr+3*Zr);

  for(int i=0; i<im.w; i++){
      for(int j=0; j<im.h; j++){
        
        L = im(i, j, 0);
        U = im(i, j, 1);
        V = im(i, j, 2);

        a = 1/3 * ((52*L)/(U+13*L*u0) -1);
        Y = (L> k*e) ? pow((L+16)/116,3.0) : L/k;
        b = -5.0 * Y;
        c = -1.0/3.0;
        d = Y*(39*L/(V+13*L*v0) - 5);

        X = (d-b)/(a-c);
        Z = X*a + b;

        im(i, j, 0) = X;
        im(i, j, 1) = Y;
        im(i, j, 2) = Z;
        
      }
    }
  

}
void xyz_to_rgb(Image& im)
{
    //PASS
    float r,g,b,X,Y,Z,R,G,B;
    float CIE_M_I[3][3] = {{2.3706743, -0.9000405, -0.4706338},
                           {-0.5138850, 1.4253036, 0.0885814},
                           {0.0052982, -0.0146949, 1.0093968}};
    for(int i=0; i<im.w; i++){
      for(int j=0; j<im.h; j++){
        
        X = im(i, j, 0);
        Y = im(i, j, 1);
        Z = im(i, j, 2);

        r = CIE_M_I[0][0]*X + CIE_M_I[0][1]*Y + CIE_M_I[0][2]*Z;
        g = CIE_M_I[1][0]*X + CIE_M_I[1][1]*Y + CIE_M_I[1][2]*Z;
        b = CIE_M_I[2][0]*X + CIE_M_I[2][1]*Y + CIE_M_I[2][2]*Z;

        R = (r<=0.0031308) ? r*12.92 : (pow(r,1/2.4)*1.055 - 0.055);
        G = (g<=0.0031308) ? g*12.92 : (pow(g,1/2.4)*1.055 - 0.055);
        B = (b<=0.0031308) ? b*12.92 : (pow(b,1/2.4)*1.055 - 0.055);

        im(i, j, 0) = R;
        im(i, j, 1) = G;
        im(i, j, 2) = B;
        
      }
    }
}
// Implementation of member functions
void Image::clamp(void) { clamp_image(*this); }
void Image::shift(int c, float v) { shift_image(*this,c,v); }
void Image::scale(int c, float v) { scale_image(*this,c,v); }

void Image::HSVtoRGB(void) { hsv_to_rgb(*this); }
void Image::RGBtoHSV(void) { rgb_to_hsv(*this); }
void Image::LCHtoRGB(void) { lch_to_rgb(*this); }
void Image::RGBtoLCH(void) { rgb_to_lch(*this); }
