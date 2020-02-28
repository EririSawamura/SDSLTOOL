#include <iostream>
#include <bits/stdc++.h>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/core/types_c.h>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/suffix_trees.hpp>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace cv;
using namespace sdsl;
using namespace std;

void test();
void test1(int i);
void test2(int i);
void test3(int i);


void compress_file(const String& file_name, const String& output_file_name);
void compress_text(const String& file_name, const String& output_file_name);
void compress_image(const String& file_name, const String& output_file_name);

void extract_file(const String& file_name, const String& output_file_name);
void extract_text(const String& file_name, const String& output_file_name);
void extract_image(const String& file_name, const String& output_file_name);
void query_text(const String& file_name);
static const char alphanum[] =
        "ab";

int stringLength = sizeof(alphanum) - 1;

char genRandom()
{
  return alphanum[rand() % stringLength];
}

int main(int argc, char ** argv ){
  String file_name = "test-data/def.png", compressed_file_name = "test.sdsl", extract_file_name = "ppt.png";
  compress_file(file_name, compressed_file_name);
  extract_file(compressed_file_name, extract_file_name);
  //test();
  //query_text(compressed_file_name);
  return 0;
}

void query_text(const String& file_name){
  csa_sada<> csa;
  load_from_file(csa, file_name);
  cout << "csa.size(): " << csa.size() << endl;
  cout << "csa.sigma : " << csa.sigma << endl;
  cout << "abba.count: " << count(csa, "abba") << endl;

  auto occs = locate(csa, "abba");
  for(auto it=csa.begin(); it!=csa.end(); ++it){
  }
  sort(occs.begin(), occs.end());
  auto max_line_length = occs[0];
  for (size_t i=1; i < occs.size(); ++i)
    max_line_length = std::max(max_line_length, occs[i]-occs[i-1]+1);
  cout << "max line length : " << max_line_length << endl;
}

void compress_file(const String& file_name, const String& output_file_name){
  if(file_name.find_last_of('.') == -1){
    compress_text(file_name, output_file_name);
  }
  else{
    string suffix = file_name.substr(file_name.find_last_of('.')+1);
    if(suffix == "txt"){
      compress_text(file_name, output_file_name);
    }
    else if(suffix == "jpg" || suffix == "png"){
      compress_image(file_name, output_file_name);
    }
  }
}

void compress_text(const String& file_name, const String& output_file_name){
  ifstream fp(file_name);
  int length;
  String buffer((std::istreambuf_iterator<char>(fp)),
                std::istreambuf_iterator<char>());;
  csa_sada<> csa;

  cout << "Compression starts." << endl;
  if (fp) {
    fp.seekg (0, std::ifstream::end);
    length = fp.tellg();
    cout << "  Original size: " << (double)length/1000000 << "mb" << endl;
    fp.seekg (0, std::ifstream::beg);
    if (fp)
      std::cout << "  all characters read successfully.." << endl;
    else
      std::cout << "  error: only " << fp.gcount() << " could be read.." << endl;

    construct_im(csa, buffer, 1);
    cout << "  Compressed size: " << (double)size_in_bytes(csa)/1000000 << "mb" << endl;
    cout << "  bp compression ratio: " << (double)length/size_in_bytes(csa) << endl;
    store_to_file(csa, output_file_name);
    fp.close();
    cout << "Compression ends. The compressed file is " << output_file_name << "." << endl;
  } else{
    return;
  }
}

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

void compress_image(const String& file_name, const String& output_file_name){
  Mat3b image = imread(file_name);
  bit_vector v(image.rows*image.cols*24 + 24);
  String bgr;

  if ( !image.data ){
    printf("No image data \n");
    return;
  }

  cout << "Compression starts: " << v.size() << endl;
  for (int i=0; i<image.rows; i++){
    for (int j=0; j<image.cols; j++) {
      Vec3b pixel = image(i, j);
      for(int k=0; k<3; k++){
        uchar bgr_ele = pixel[k];
        for(int l=sizeof(unsigned char)*8-1; l; bgr_ele>>=1u, l--){
          v[i*image.cols*24 + j*24 + k*8 + l] = bgr_ele&1u;
        }
      }
    }
  }

  //Add image size information
  int h = image.rows, w = image.cols;
  for(long i=image.rows*image.cols*24+11; i>=image.rows*image.cols*24; i-=1){
    v[i] = h%2;
    h /= 2;
    v[i+12] = w%2;
    w /= 2;
  }

  sd_vector<> bps(v);
  cout << "Compression ends." << endl;
  store_to_file(bps, output_file_name);
  cout << "  bp compression ratio: " << (double)v.size()/(double)1000000/(double)size_in_mega_bytes(bps) << endl;
  cout << "Compression ends. The compressed file is " << output_file_name << "." << endl;
}

void extract_file(const String& file_name, const String& output_file_name){
  if(output_file_name.find_last_of('.') == -1){
    extract_text(file_name, output_file_name);
  }
  else{
    string suffix = output_file_name.substr(output_file_name.find_last_of('.')+1);
    if(suffix == "txt"){
      extract_text(file_name, output_file_name);
    }
    else if(suffix == "jpg" || suffix == "png"){
      extract_image(file_name, output_file_name);
    }
  }
}

void extract_text(const String& file_name, const String& output_file_name){
  csa_sada<> csa;

  cout << "Extraction starts." << endl;
  load_from_file(csa, file_name);
  ofstream fp(output_file_name);
  if(fp){
    fp << extract(csa, 0, csa.size()-2);
    fp.close();
  }else{
    return;
  }
  cout << "Extraction ends. The compressed file is " << output_file_name << "." << endl;
}

void extract_image(const String& file_name, const String& output_file_name){
  cout << "extract image starts" << endl;
  int width=0, height=0, count=0;
  sd_vector<> bps;
  load_from_file(bps, file_name);
  cout << size_in_mega_bytes(bps) << " " << endl;
  cout << bps.size() << endl;

  //Extract image size
  for(long i=0; i<=11; i+=1){
    height *= 2;
    height += bps[bps.size()-24+i];
    width *= 2;
    width += bps[bps.size()-24+i+12];
  }
  cout << width << " " << height;
  Mat3b image(height, width, CV_8UC3);

  for (int i=0; i<height; i++){
    for (int j=0; j<width; j++) {
      for(int k=0; k<3; k++){
        uchar bgr_ele = 0u;
        for(unsigned long l=0; l<=sizeof(unsigned char)*8-1; l+=1){
          bgr_ele <<= 1u;
          bgr_ele = bgr_ele | bps[i*image.cols*24 + j*24 + k*8 + l];
        }
        image(i, j)[k] = bgr_ele | 128u;
      }
      Vec3b pixel = image(i, j);
    }
  }

  cout << "-------------------" << endl;

  for (int i=0; i<3; i++){
    for (int j=0; j<3; j++) {
      Vec3b pixel = image(i, j);
      cout << +pixel[0] << " " << +pixel[1] << " " << +pixel[2] << endl;
    }
  }
  cout << "-------------------" << endl;
  imwrite(output_file_name, image);
}

void test4(int i){
  ifstream fp("test-data/test"+to_string(i));
  int length;
  String buffer((std::istreambuf_iterator<char>(fp)),
                std::istreambuf_iterator<char>());;
  csa_sada<> csa;
  if (fp) {
    fp.seekg (0, std::ifstream::end);
    length = fp.tellg();
    fp.seekg (0, std::ifstream::beg);
    construct_im(csa, buffer, 1);
    cout << "  bp compression ratio: " << (double)length/size_in_bytes(csa) << endl;
    fp.close();
  } else{
    return;
  }
}

void test5(int i){
  ifstream fp("test-data/test"+to_string(i));
  int length;
  String buffer((std::istreambuf_iterator<char>(fp)),
                std::istreambuf_iterator<char>());;
  cst_sct3<> csa;
  if (fp) {
    fp.seekg (0, std::ifstream::end);
    length = fp.tellg();
    fp.seekg (0, std::ifstream::beg);
    construct_im(csa, buffer, 1);
    cout << "  bp compression ratio: " << (double)length/size_in_bytes(csa) << endl;
    fp.close();
  } else{
    return;
  }
}

void test6(int i){
  ifstream fp("test-data/test"+to_string(i));
  int length;
  String buffer((std::istreambuf_iterator<char>(fp)),
                std::istreambuf_iterator<char>());;
  wt_hutu<rrr_vector<63>> csa;
  if (fp) {
    fp.seekg (0, std::ifstream::end);
    length = fp.tellg();
    fp.seekg (0, std::ifstream::beg);
    construct_im(csa, buffer, 1);
    cout << "  bp compression ratio: " << (double)length/size_in_bytes(csa) << endl;
    fp.close();
  } else{
    return;
  }
}

void test(){
  //Mat3b image = imread("test-data/ppp.png");
  //imwrite("ppp.png", image);
  bit_vector v(24);
  int w=720, h=255;

  for(int i=11; i>=0; i--){
    v[i] = w%2;
    w /= 2;
    v[i+12] = h%2;
    h /= 2;
  }
  //cout << v;
  int width=0, height=0;
  for(int i=0; i<=11; i++){
    width *= 2;
    width += v[i];
    height *= 2;
    height += v[i+12];
  }
  cout << width << " " << height;
  /*for(int i=1; i<=4; i++){
    ofstream fp("test-data/test" + to_string(i));
    srand(time(0));
    for(int j=1; j<=1000*pow(10, i); j++)
      fp << genRandom();
    fp.close();
  }
  for(int i=1; i<=4; i++){
    cout << "=========================" << endl;
    test4(i);
    test5(i);
    test6(i);
    cout << "=========================" << endl;
  }*/
}

void test1(int i) {
  bit_vector v(8000 * pow(10, i));
  for (size_t j=0; j < v.size(); j+=1) {
    v[j] = rand() % 2 == 1;
    /*v[j] = false;
    v[j+1] = true;
    v[j+2] = true;
    v[j+3] = false;
    v[j+4] = false;
    v[j+5] = false;
    v[j+6] = false;
    v[j+7] = true;
    v[j+8] = false;
    v[j+9] = true;
    v[j+10] = true;
    v[j+11] = false;
    v[j+12] = false;
    v[j+13] = false;
    v[j+14] = true;
    v[j+15] = false;
    v[j+16] = false;
    v[j+17] = true;
    v[j+18] = true;
    v[j+19] = false;
    v[j+20] = false;
    v[j+21] = false;
    v[j+22] = true;
    v[j+23] = true;
    v[j+24] = false;
    v[j+25] = true;
    v[j+26] = true;
    v[j+27] = false;
    v[j+28] = false;
    v[j+29] = true;
    v[j+30] = false;
    v[j+31] = false;*/
  }
  rank_support_v<> bps(&v);
  cout << "  compression ratio: " << (double)v.size()/(double)1000000/(double)size_in_mega_bytes(bps) << endl;
}

void test2(int i) {
  bit_vector v(8000 * pow(10, i));
  for (size_t j=0; j < v.size(); j+=1) {
    v[j] = rand() % 2 == 1;
    /*v[j] = false;
    v[j+1] = true;
    v[j+2] = true;
    v[j+3] = false;
    v[j+4] = false;
    v[j+5] = false;
    v[j+6] = false;
    v[j+7] = true;
    v[j+8] = false;
    v[j+9] = true;
    v[j+10] = true;
    v[j+11] = false;
    v[j+12] = false;
    v[j+13] = false;
    v[j+14] = true;
    v[j+15] = false;
    v[j+16] = false;
    v[j+17] = true;
    v[j+18] = true;
    v[j+19] = false;
    v[j+20] = false;
    v[j+21] = false;
    v[j+22] = true;
    v[j+23] = true;
    v[j+24] = false;
    v[j+25] = true;
    v[j+26] = true;
    v[j+27] = false;
    v[j+28] = false;
    v[j+29] = true;
    v[j+30] = false;
    v[j+31] = false;*/
  }
  bp_support_sada<> sada(&v);
  cout << "  compression ratio: " << (double)v.size()/(double)1000000/(double)size_in_mega_bytes(sada) << endl;
}

void test3(int i) {
  bit_vector v(8000 * pow(10, i));
  for (size_t j=0; j < v.size(); j+=1) {
    v[j] = rand() % 2 == 1;
    /*v[j] = false;
    v[j+1] = true;
    v[j+2] = true;
    v[j+3] = false;
    v[j+4] = false;
    v[j+5] = false;
    v[j+6] = false;
    v[j+7] = true;
    v[j+8] = false;
    v[j+9] = true;
    v[j+10] = true;
    v[j+11] = false;
    v[j+12] = false;
    v[j+13] = false;
    v[j+14] = true;
    v[j+15] = false;
    v[j+16] = false;
    v[j+17] = true;
    v[j+18] = true;
    v[j+19] = false;
    v[j+20] = false;
    v[j+21] = false;
    v[j+22] = true;
    v[j+23] = true;
    v[j+24] = false;
    v[j+25] = true;
    v[j+26] = true;
    v[j+27] = false;
    v[j+28] = false;
    v[j+29] = true;
    v[j+30] = false;
    v[j+31] = false;*/
  }
  sd_vector<> sada(v);
  cout << "  compression ratio: " << (double)v.size()/(double)1000000/(double)size_in_mega_bytes(sada) << endl;
}

