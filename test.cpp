#include <fstream>
#include <iostream> 
#include <vector>

using namespace std;

void load_data(char* filename, float*& data, unsigned& num, unsigned& dim) { 
  std::ifstream in(filename, std::ios::binary);	
  if (!in.is_open()) {
    std::cout << "open file error" << std::endl;
    exit(-1);
  }
  in.read((char*)&dim, 4);	//读取向量维度
  in.seekg(0, std::ios::end);	//光标定位到文件末尾
  std::ios::pos_type ss = in.tellg();	//获取文件大小（多少字节）
  size_t fsize = (size_t)ss;
  num = (unsigned)(fsize / (dim + 1) / 4);	//数据的个数
  data = new float[(size_t)num * (size_t)dim];

  in.seekg(0, std::ios::beg);	//光标定位到起始处
  for (size_t i = 0; i < num; i++) {
    in.seekg(4, std::ios::cur);	//光标向右移动4个字节
    in.read((char*)(data + i * dim), dim * 4);	//读取数据到一维数据data中
  }
  for(size_t i = 0; i < 10 * dim; i++) {	//输出数据
    std::cout << (float)data[i];
    if(!i) {
        std::cout << " ";
        continue;
    }
    if(i % (dim - 1) != 0) {
      std::cout << " ";
    }
    else{
      std::cout << std::endl;
    }
  }
  in.close();
}

void Load_nn_graph(const char *filename) {
  std::ifstream in(filename, std::ios::binary);
  unsigned k;
  in.read((char *)&k, sizeof(unsigned));
  in.seekg(0, std::ios::end);
  std::ios::pos_type ss = in.tellg();
  size_t fsize = (size_t)ss;
  size_t num = (unsigned)(fsize / (k + 1) / 4);
  in.seekg(0, std::ios::beg);
  std::vector<std::vector<unsigned > > final_graph_;

  final_graph_.resize(num);
  final_graph_.reserve(num);
  unsigned kk = (k + 3) / 4 * 4;
  for (size_t i = 0; i < num; i++) {
    in.seekg(4, std::ios::cur);
    final_graph_[i].resize(k);
    final_graph_[i].reserve(kk);
    in.read((char *)final_graph_[i].data(), k * sizeof(unsigned));
  }
  std::cout <<k<<" "<<num<<" "<<kk<< std::endl;
  //for(int j=0;j<10;j++)
  	for(size_t i = 0; i< final_graph_[443243].size() ;i++)
  		std::cout <<i<<"   "<<(unsigned)final_graph_[443243][i]<< std::endl;
  
//  for(size_t i = 0; i < num; i++) {	//输出数据
//    std::cout << (unsigned)final_graph_[i].data();
//    if(!i) {
//        std::cout << " ";
//        continue;
//    }
//    if(i % (k - 1) != 0) {
//      std::cout << " ";
//    }
//    else{
//      std::cout << std::endl;
//    }
//  }
  
  in.close();
}

void load_result(char* filename){
	vector<vector<unsigned > > result;
	unsigned K;
  	ifstream in(filename,ios::binary);
  	in.read((char *)&K, sizeof(unsigned));
 	in.seekg(0, ios::end);
  	ios::pos_type ss = in.tellg();
  	size_t fsize = (size_t)ss;
  	size_t num = (unsigned)(fsize / (K + 1) / 4);  //计算数据个数
  	in.seekg(0, ios::beg);

  	result.resize(num);
  	for(size_t i = 0; i < num; i++){
    	in.seekg(4, ios::cur);
    	result[i].resize(K);
    	in.read((char *)result[i].data(), K * sizeof(unsigned));  //读取数据 
  	}
  	for(size_t j=0;j<num;j++){
  		cout<<"query"<<j<<":"<<endl;
  		for(size_t i = 0; i<result[j].size() ;i++)
  			cout <<i<<"   "<<(unsigned)result[j][i]<<endl;
  		cout<<endl;
	}	
  	in.close();
}


int main(int argc, char** argv) {
//  float* data_load = NULL;
//  unsigned points_num, dim;
//  char name[] = "sift_base.fvecs";
//  char name2[] = "sift_100NN_100.graph";
//  char name3[] = "sift_query.fvecs";
//  Load_nn_graph(name2);
//  //load_data(name, data_load, points_num, dim);
//  std::cout << "points_num："<< points_num << std::endl << "data dimension：" << dim << std::endl;
	load_result("res/gist_10_accurate_res2.ivecs");
  return 0;
}
