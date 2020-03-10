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
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace cv;
using namespace sdsl;
using namespace std;
using namespace std::chrono;

void test();
void text_compression_test();
void text_compression_time_test();
void image_compression_test();
void query_measurement_test();
void query_measurement_test1();
void test1(int i);
void test2(int i);
void test3(int i);
void test4(int i);
void test5(int i);
void test6(int i);


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
  String file_name = "test-data/ppp.png", compressed_file_name = "test.sdsl", extract_file_name = "test3.png";
  //compress_file(file_name, compressed_file_name);
  //extract_file(compressed_file_name, extract_file_name);
  query_measurement_test1();

  //text_compression_time_test();
  //text_compression_test();
  //image_compression_test();
  //query_text(compressed_file_name);
  return 0;
}

void query_text(const String& file_name){
  csa_sada<> csa;
  load_from_file(csa, file_name);
  cout << "csa.size(): " << csa.size() << endl;
  cout << "csa.sigma : " << csa.sigma << endl;
  cout << "abba.count: " << count(csa, "ab") << endl;

  auto occs = locate(csa, "ab");
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

  std::filebuf ot;
  ot.open (output_file_name, std::ios::out);
  std::ostream os(&ot);

  String buffer((std::istreambuf_iterator<char>(fp)),std::istreambuf_iterator<char>());;
  wt_hutu <> csa;

  cout << "Compression starts." << endl;
  if (fp) {
    if (fp)
      std::cout << "  all characters read successfully.." << endl;
    else
      std::cout << "  error: only " << fp.gcount() << " could be read.." << endl;

    construct_im(csa, buffer, 1);
    csa.serialize(os);

    ot.close();
    fp.close();
    cout << "Compression ends. The compressed file is " << output_file_name << "." << endl;
  } else{
    return;
  }

}

void compress_image(const String& file_name, const String& output_file_name){
  cv::Mat image = cv::imread(file_name, cv::IMREAD_UNCHANGED);
  bit_vector v(image.rows*image.cols*24 + 36);
  String bgr;

  if ( !image.data ){
    printf("No image data \n");
    return;
  }

  cout << "Compression starts." << endl;
  for (int i=0; i<image.rows; i++){
    for (int j=0; j<image.cols; j++) {
      unsigned char * pixel = image.ptr(i, j);
      for(int k=0; k<3; k++){
        uchar bgr_ele = pixel[k];
        for(int l=sizeof(unsigned char)*8-1; l>=0; bgr_ele>>=1u, l--){
          v[i*image.cols*24 + j*24 + k*8 + l] = bgr_ele&1u;
        }
      }
    }
  }

  //Add image size information
  int h = image.rows, w = image.cols, t = image.type();
  for(long i=image.rows*image.cols*24+11; i>=image.rows*image.cols*24; i-=1){
    v[i] = h%2;
    h /= 2;
    v[i+12] = w%2;
    w /= 2;
    v[i+24] = t%2;
    t /= 2;
  }

  std::filebuf ot;
  ot.open (output_file_name, std::ios::out);
  std::ostream os(&ot);

  bp_support_sada<> bps(&v);

  bps.serialize(os);

  ot.close();
  //store_to_file(bps, output_file_name);
  cout << "  Image compression ratio: " << (double)v.size()/(double)8000000/(double)size_in_mega_bytes(bps) << endl;
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
  wt_hutu <> csa;
  std::filebuf fb;

  cout << "Extraction starts." << endl;

  ofstream fp(output_file_name);
  if (fb.open (file_name, std::ios::in))
  {
    std::istream ifa(&fb);
    csa.load(ifa);

    fb.close();
  }
  if(fp){
    fp << csa;
    fp.close();
  }else{
    return;
  }
  cout << "Extraction ends. The extracted file is " << output_file_name << "." << endl;
}

void extract_image(const String& file_name, const String& output_file_name){
  int width=0, height=0, type=0;
  bp_support_sada<> bps;
  ifstream fp(file_name);

  cout << "Extraction starts." << endl;
  std::filebuf fb;
  if (fb.open (file_name, std::ios::in))
  {
    std::istream ifa(&fb);
    //bps.load(&ifa);

    fb.close();
  }
  //load_from_file(bps, file_name);
  cout << bps.rank(0) << endl;
  return;
  //Extract image size
  /*for(long i=0; i<=11; i+=1){
    height *= 2;
    height += bps(bps.size()-36+i);
    width *= 2;
    width += bps(bps.size()-24+i);
    type *= 2;
    type += bps(bps.size()-12+i);
  }

  cout << height << " " << width << endl;

  cv::Mat image(height, width, type);
  uchar set[8] = {1, 2, 4, 8, 16, 32, 64, 128};

  for (int i=0; i<height; i++){
    for (int j=0; j<width; j++) {
      for(int k=0; k<3; k++){
        uchar bgr_ele = 0;
        for(unsigned long l=0; l<=sizeof(unsigned char)*8-1; l+=1){
          if(bps(i*width*24 + j*24 + k*8 + l))
            bgr_ele = bgr_ele + set[7-l];
        }
        unsigned char * p = image.ptr(i, j); // Y first, X after
        p[k] = bgr_ele;
      }
    }
  }

  imwrite(output_file_name, image);
  cout << "Extraction ends. The compressed file is " << output_file_name << "." << endl;*/
}

//Measure text compression ratio
void text_compression_test(){
  for(int i=1; i<=4; i++){
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
  }
}

//Measure text compression time
void text_compression_time_test(){
  String file_name = "test-data/test4", compressed_file_name = "test.sdsl", extract_file_name = "ppt.png";

  auto start = high_resolution_clock::now();
  compress_file(file_name, compressed_file_name);
  auto end = high_resolution_clock::now();
  cout << "Locate function counts " <<
       duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;
}

//Measure image compression ratio
void image_compression_test(){
  for(int i=1; i<=4; i++){
    ofstream fp("test-data/test" + to_string(i));
    srand(time(0));
    for(int j=1; j<=1000*pow(10, i); j++)
      fp << genRandom();
    fp.close();
  }
  for(int i=1; i<=4; i++){
    cout << "=========================" << endl;
    test1(i);
    test2(i);
    test3(i);
    cout << "=========================" << endl;
  }
}

//Measure query function execution time for the compressed suffix array
void query_measurement_test(){
  //Create test file
  for(int i=1; i<=4; i++){
    ofstream fp("test-data/test" + to_string(i));
    srand(time(0));
    for(int j=1; j<=1000*pow(10, i); j++){
      if(j%3 == 1){
        fp << "a";
      }else if(j%3 == 2){
        fp << "b";
      }else if(j%3 == 0){
        fp << "c";
      }
    }
    fp.close();
  }

  //Measure time
  for(int i=1; i<=4; i++){
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

      //Measure time
      auto start = high_resolution_clock::now();
      cout << "size: " << csa.size() << endl;
      auto end = high_resolution_clock::now();
      cout << "size function counts " <<
           duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

      start = high_resolution_clock::now();
      count(csa, "abcabcab");
      end = high_resolution_clock::now();
      cout << "Count function counts " <<
           duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

      start = high_resolution_clock::now();
      auto occs = locate(csa, "abcabcab");
      end = high_resolution_clock::now();
      cout << "Locate function counts " <<
           duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;
      cout << "---------------------" << endl;
      fp.close();
    } else{
      return;
    }
  }
}

//Measure query function execution time for the wavelet tree
void query_measurement_test1(){
  //Create test file
  for(int i=1; i<=4; i++){
    ofstream fp("test-data/test" + to_string(i));
    srand(time(0));
    for(int j=1; j<=1000*pow(10, i); j++){
      if(j%3 == 1){
        fp << "a";
      }else if(j%3 == 2){
        fp << "b";
      }else if(j%3 == 0){
        fp << "c";
      }
    }
    fp.close();
  }

  //Measure time
  for(int i=1; i<=4; i++){
    ifstream fp("test-data/test"+to_string(i));
    int length;
    String buffer((std::istreambuf_iterator<char>(fp)),
                  std::istreambuf_iterator<char>());;
    wt_hutu<> csa;
    if (fp) {
      fp.seekg (0, std::ifstream::end);
      length = fp.tellg();
      fp.seekg (0, std::ifstream::beg);
      construct_im(csa, buffer, 1);

      //Measure time
      auto start = high_resolution_clock::now();
      cout << "size: " << csa.size() << endl;
      auto end = high_resolution_clock::now();
      cout << "size function counts " <<
           duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

      //! Calculates how many symbols c are in the prefix [0..i-1].
      start = high_resolution_clock::now();
      uint64_t r = csa.rank(csa.size(), csa[1]);
      end = high_resolution_clock::now();
      cout << "Rank function counts " <<
           duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

      //! Calculates the ith occurrence of the symbol c in the supported vector.
      start = high_resolution_clock::now();
      csa.select(r, csa[1]);
      end = high_resolution_clock::now();
      cout << "Select function counts " <<
           duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;
      cout << "---------------------" << endl;
      fp.close();
    } else{
      return;
    }
  }
}

void test4(int i){
  ifstream fp("test-data/test"+to_string(i));
  int length;
  String buffer((std::istreambuf_iterator<char>(fp)),
                std::istreambuf_iterator<char>());;
  csa_sada<> csa;
  csa_bitcompressed<> csb;
  csa_wt<> csc;
  if (fp) {
    fp.seekg (0, std::ifstream::end);
    length = fp.tellg();
    fp.seekg (0, std::ifstream::beg);
    construct_im(csa, buffer, 1);
    cout << "csa_sada compression ratio: " << (double)length/size_in_bytes(csa) << endl;

    construct_im(csb, buffer, 1);
    cout << "csa_bitcompressed compression ratio: " << (double)length/size_in_bytes(csb) << endl;

    construct_im(csc, buffer, 1);
    cout << "csa_wt compression ratio: " << (double)length/size_in_bytes(csc) << endl;
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
  cst_sada<> csb;

  if (fp) {
    fp.seekg (0, std::ifstream::end);
    length = fp.tellg();
    fp.seekg (0, std::ifstream::beg);

    construct_im(csa, buffer, 1);
    cout << "cst_sct3 compression ratio: " << (double)length/size_in_bytes(csa) << endl;

    construct_im(csb, buffer, 1);
    cout << "cst_sada compression ratio: " << (double)length/size_in_bytes(csb) << endl;
    fp.close();
  } else{
    return;
  }
}

void test6(int i){
  ifstream fp("test-data/test"+to_string(i));
  int length;
  String buffer((std::istreambuf_iterator<char>(fp)),
                std::istreambuf_iterator<char>());

  wt_rlmn<> csa;
  wt_gmr<> csb;
  wt_ap<> csc;
  wt_huff<> csd;
  wm_int<> cse;
  wt_blcd<> csf;
  wt_hutu<> csg;
  wt_int<> csh;

  if (fp) {
    fp.seekg (0, std::ifstream::end);
    length = fp.tellg();
    fp.seekg (0, std::ifstream::beg);

    construct_im(csa, buffer, 1);
    cout << "wt_rlmn compression ratio: " << (double)length/size_in_bytes(csa) << endl;

    construct_im(csb, buffer, 1);
    cout << "wt_gmr compression ratio: " << (double)length/size_in_bytes(csb) << endl;

    construct_im(csc, buffer, 1);
    cout << "wt_ap compression ratio: " << (double)length/size_in_bytes(csc) << endl;

    construct_im(csd, buffer, 1);
    cout << "wt_huff compression ratio: " << (double)length/size_in_bytes(csd) << endl;

    construct_im(cse, buffer, 1);
    cout << "wm_int compression ratio: " << (double)length/size_in_bytes(cse) << endl;

    construct_im(csf, buffer, 1);
    cout << "wt_blcd compression ratio: " << (double)length/size_in_bytes(csf) << endl;

    construct_im(csg, buffer, 1);
    cout << "wt_hutu compression ratio: " << (double)length/size_in_bytes(csg) << endl;

    construct_im(csh, buffer, 1);
    cout << "wt_int compression ratio: " << (double)length/size_in_bytes(csh) << endl;
    fp.close();
  } else{
    return;
  }
}

void test(){
  uchar bgr_ele = 127u;

  for(int l=sizeof(unsigned char)*8-1; l; bgr_ele>>=1u, l--){
    bgr_ele = bgr_ele&1u;
  }
  uchar output = 0u;
  for(unsigned long l=0; l<sizeof(unsigned char)*8-1; l+=1){
    //output <<= 1;
    cout << +output << endl;
    output = bgr_ele >>= 1u;
  }
  cout << +output;
  //image(i, j)[k] = bgr_ele | 128u;
  //cout << +bgr_ele;
  //String file_name = "test-data/ppp.png", compressed_file_name = "test.sdsl", extract_file_name = "ppt.png";
  //cv::Mat image = cv::imread(file_name, cv::IMREAD_UNCHANGED);
  //imwrite("test.png", image);

  //Mat3b image = imread(file_name);
  /*bit_vector v(image.rows*image.cols*24 + 24);
  String bgr;

  if ( !image.data ){
    printf("No image data \n");
    return;
  }

  unsigned char * pixel = image.ptr(100, 100);
  for(int i=0; i<24; i++){
    cout << +pixel[i];
  }
  cout << "------" << endl;
  pixel = image.ptr(100, 101);
  for(int i=0; i<24; i++){
    cout << +pixel[i];
  }*/
  //Mat3b image = imread("test-data/ppp.png");
  //imwrite("ppp.png", image);
  /*bit_vector v(24);
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
  cout << width << " " << height;*/

}

void test1(int i) {
  bit_vector v(8000 * pow(10, i));
  for (size_t j=0; j < v.size(); j+=1) {
    v[j] = rand() % 2 == 1;
  }
  sd_vector<> sada(v);
  bit_vector_il<> sadb(v);
  hyb_vector<> sadc(v);
  rrr_vector<> csbb(v);

  auto start = high_resolution_clock::now();
  rank_support_v<> bpa(&v);
  auto end = high_resolution_clock::now();
  cout << "rank_support_v compression ratio: " << (double)v.size()/(double)8000000/(double)size_in_mega_bytes(bpa) <<
  " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

  start = high_resolution_clock::now();
  rank_support_v5<> bpb(&v);
  end = high_resolution_clock::now();
  cout << "rank_support_v5 compression ratio: " << (double)v.size()/(double)8000000/(double)size_in_mega_bytes(bpb) <<
  " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

  start = high_resolution_clock::now();
  rank_support_scan<> bpc(&v);
  end = high_resolution_clock::now();
  cout << "rank_support_scan compression ratio: " << (double)v.size()/(double)8000000/(double)size_in_mega_bytes(bpc) <<
  " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" <<endl;

  start = high_resolution_clock::now();
  rank_support_il<> bpd(&sadb);
  end = high_resolution_clock::now();
  cout << "rank_support_il compression ratio: " << (double)v.size()/(double)8/(double)size_in_bytes(bpd) <<
  " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

  start = high_resolution_clock::now();
  rank_support_rrr<> bpe(
          reinterpret_cast<const rank_support_rrr<1, 15, int_vector<'\x00'>, 32>::bit_vector_type *>(&csbb));
  end = high_resolution_clock::now();
  cout << "rank_support_rrr compression ratio: " << (double)v.size()/(double)8/(double)size_in_bytes(bpe) <<
  " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

  start = high_resolution_clock::now();
  rank_support_sd<> bpf(&sada);
  end = high_resolution_clock::now();
  cout << "rank_support_sd compression ratio: " << (double)v.size()/(double)8/(double)size_in_bytes(bpf) <<
  " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

  start = high_resolution_clock::now();
  rank_support_hyb<> bpg(&sadc);
  end = high_resolution_clock::now();
  cout << "rank_support_hyb compression ratio: " << (double)v.size()/(double)8/(double)size_in_bytes(bpg) <<
  " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

}

void test2(int i) {
  bit_vector v(8000 * pow(10, i));
  for (size_t j=0; j < v.size(); j+=1) {
    v[j] = rand() % 2 == 1;
  }

  auto start = high_resolution_clock::now();
  bp_support_sada<> sada(&v);
  auto end = high_resolution_clock::now();
  cout << "bp_support_sada compression ratio: " << (double)v.size()/(double)8000000/(double)size_in_mega_bytes(sada) <<
  " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

  /*start = high_resolution_clock::now();
  bp_support_g<> sadb(&v);
  end = high_resolution_clock::now();
  cout << "bp_support_g compression ratio: " << (double)v.size()/(double)1000000/(double)size_in_mega_bytes(sadb) <<
       " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;*/

  start = high_resolution_clock::now();
  bp_support_gg<> sadc(&v);
  end = high_resolution_clock::now();
  cout << "bp_support_gg compression ratio: " << (double)v.size()/(double)8000000/(double)size_in_mega_bytes(sadc) <<
       " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;
}

void test3(int i) {
  bit_vector v(8000 * pow(10, i));
  for (size_t j=0; j < v.size(); j+=1) {
    v[j] = rand() % 2 == 1;
  }

  auto start = high_resolution_clock::now();
  sd_vector<> sa(v);
  auto end = high_resolution_clock::now();
  cout << "sd_vector compression ratio: " << (double)v.size()/(double)8000000/(double)size_in_mega_bytes(sa) <<
       " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

  start = high_resolution_clock::now();
  bit_vector_il<> sb(v);
  end = high_resolution_clock::now();
  cout << "bit_vector_li compression ratio: " << (double)v.size()/(double)8000000/(double)size_in_mega_bytes(sb) <<
       " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

  start = high_resolution_clock::now();
  rrr_vector<> sc(v);
  end = high_resolution_clock::now();
  cout << "rrr_vector compression ratio: " << (double)v.size()/(double)8000000/(double)size_in_mega_bytes(sc) <<
       " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;

  start = high_resolution_clock::now();
  hyb_vector<> sd(v);
  end = high_resolution_clock::now();
  cout << "hyb_vector compression ratio: " << (double)v.size()/(double)8000000/(double)size_in_mega_bytes(sd) <<
       " Time: " << duration_cast<microseconds>(end - start).count() / 1000 << "ms" << endl;
}

