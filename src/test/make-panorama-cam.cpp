#include "../image.h"
#include "../utils.h"
#include "../matrix.h"

#include <string>

#define DEFAULT_NUM_PHOTOS 2
#define DEFAULT_SCALE_COEF 3

using namespace std;

string indir="cam/";
string outdir="cam/out/";

void load_images(image_map& im, const string& indir, const string& outdir, int numpics){
  TIME(1);
  im.indir=indir;
  im.outdir=outdir;
  vector<unique_ptr<thread>> th; 
  for(int i=0; i<numpics; i++)th.emplace_back(new thread([&,i]()
    {
    string file=indir+"image"+to_string(i)+"_reduced"+".jpg";
    Image in=load_image(file);
    printf("%s loaded into im[\"%s\"]\n",file.c_str(),to_string(i).c_str());
    }));
  for(auto&e1:th)e1->join();th.clear();
}
  
void resizeBilinear(int num_photos, int coeff){
  for(int i=0; i<num_photos; i++){ 
    string inFile = indir+"image"+to_string(i)+".jpg";
    string outFile = indir+"image"+to_string(i)+"_reduced";
    Image in = load_image(inFile);
    in = bilinear_resize(in, in.w/coeff, in.h/coeff);
    save_png(in, outFile);
  }
  return;
}

void create_panorama(image_map& im, const string& out, const string& aname, const string& bname,
                     float sigma, int corner_method, float thresh, int window, int nms, float inlier_thresh, int iters, int cutoff, float acoeff){
  printf("Combining %s and %s into %s...\n",aname.c_str(),bname.c_str(),out.c_str());
  assert(im[aname].size()!=0 && "Image A invalid\n");
  assert(im[bname].size()!=0 && "Image B invalid\n");
  im[out]=panorama_image(im[aname],im[bname],sigma,corner_method,thresh,window,nms,inlier_thresh,iters,cutoff,acoeff);
  save_png(im[out],im.outdir+out);
  printf("%s finished computing\n",out.c_str());
}
  
void makePanorama(int num_photos){
  string indir="cam/";
  string outdir="cam/out/";
  
  image_map im;
  load_images(im,indir,outdir,num_photos); 
  vector < thread > th; 
  
  int pairs = num_photos/2;
  for(int i=0; i<pairs; i++){
    th.push_back(thread([&](){create_panorama(im,"inter"+to_string(i),to_string(2*i),to_string(2*i+1) ,3,0,0.15,13,15,9,1000,100,0);})); 
  }
  for(auto&e1:th)e1.join();th.clear();
  if(num_photos%2==1){
    create_panorama(im,"inter"+to_string(pairs-1),"inter"+to_string(pairs-1),to_string(num_photos-1) ,3,0,0.15,13,15,9,1000,100,0);
  }
  //TODO: reduce size here
  if(num_photos==5 || num_photos==4){
    create_panorama(im,"out","inter0",to_string(num_photos-1) ,3,0,0.15,13,15,9,1000,100,0);
  }
  else if(num_photos==3 || num_photos==2){
    //nothing
  }
  save_images(im,outdir);
}

int main(int argc, char **argv){
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
