#include "../image.h"
#include "../utils.h"
#include "../matrix.h"

#include <string>

#define DEFAULT_NUM_PHOTOS 2
#define DEFAULT_SCALE_COEF 3

using namespace std;

string indir="cam/";
string outdir="cam/out/";

void resizeBilinear(int num_photos, int coeff){
  for(int i=0; i<num_photos; i++){ 
    string inFile = indir+"image"+to_string(i)+".jpg";
    string outFile = indir+"image"+to_string(i)+"_reduced"+to_string(coeff);
    Image in = load_image(inFile);
    in = bilinear_resize(in, in.w/coeff, in.h/coeff);
    save_png(in, outFile);
  }
  return;
}

int main(int argc, char **argv)
{
  int num_photos;
  int scale_coeff;
  if(argc<=2){
    printf("using default parameters [num_photos=%d] [scale_coeff=%d]\n", DEFAULT_NUM_PHOTOS, DEFAULT_SCALE_COEF);
    num_photos = DEFAULT_NUM_PHOTOS;
    scale_coeff = DEFAULT_SCALE_COEF;
  }
  else{
    num_photos = stoi(argv[1]);
    scale_coeff = stoi(argv[2]);
  }
  printf("Number of photos: %d\nScale coeff: %d\n", num_photos, scale_coeff);

  resizeBilinear(num_photos, scale_coeff);

  return 0;
}
