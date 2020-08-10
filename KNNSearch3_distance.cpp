#include "KNNSearch.h"
#include "distance.h"
#define QUERY_NUM 200

using namespace std;

void KNNSearch::load_data(char* filename,vector<vector<float > >& data){
	ifstream in(filename, ios::binary);
  	if (!in.is_open()){
    	cout<<"open file error"<<endl;
    	exit(-1);
  	}
  	in.read((char*)&dim, 4);	//读取向量维度
  	in.seekg(0, ios::end);	
  	ios::pos_type ss = in.tellg();	//获取文件大小（多少字节）
  	size_t fsize = (size_t)ss;
  	unsigned num = (unsigned)(fsize / (dim + 1) / 4);	//计算数据个数
  	data.resize(num);
  	data.reserve(num);

  	in.seekg(0, ios::beg);
 	for(size_t i = 0; i < num; i++){
    	in.seekg(4, ios::cur);
    	data[i].resize(dim);
    	in.read((char*)data[i].data(), dim * sizeof(float));	//读取数据到data中
  	}
	in.close();
}


void KNNSearch::load_graph(char* filename){
  	ifstream in(filename,ios::binary);
  	in.read((char *)&k, sizeof(unsigned));
 	in.seekg(0, ios::end);
  	ios::pos_type ss = in.tellg();
  	size_t fsize = (size_t)ss;
  	size_t num = (unsigned)(fsize / (k + 1) / 4);  //计算数据个数
  	in.seekg(0, ios::beg);

  	knn_graph.resize(num);
  	knn_graph.reserve(num);
  	unsigned kk = (k + 3) / 4 * 4;
  	for(size_t i = 0; i < num; i++){
    	in.seekg(4, ios::cur);
    	knn_graph[i].resize(k);
    	knn_graph[i].reserve(kk);
    	in.read((char *)knn_graph[i].data(), k * sizeof(unsigned));  //读取数据 
  	}
  	in.close();
}


void KNNSearch::save_result(char* filename,	vector<vector<unsigned > >& result){
  	ofstream out(filename, ios::binary | ios::out);

  	for (unsigned i = 0; i < result.size(); i++) {
    	out.write((char*)&K, sizeof(unsigned));
    	out.write((char*)result[i].data(), K * sizeof(unsigned));
 	 }
  	out.close();
}


void KNNSearch::load_result(char* filename,	vector<vector<unsigned > >& result,int choice){
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
  	if(choice == 1){
  		for(size_t j=0;j<num;j++){
  			cout<<"query"<<j<<":"<<endl;
  			for(size_t i = 0; i<result[j].size() ;i++)
  				cout <<i<<"   "<<(unsigned)result[j][i]<<endl;
  			cout<<endl;
		}	
	}
  	in.close();
}


static bool LessSort(Candidate_List a,Candidate_List b){
	return (a.dis<b.dis);
} 


void KNNSearch::Search(){
	vector<Candidate_List> pool(L);  //定义候选池
	srand((int)time(0));  //设置随机数种子 
	//size_t query_num = query_data.size();
	size_t query_num = QUERY_NUM;
	size_t base_num = base_data.size();
	res.resize(query_num);
	DistanceL2* distance_ = new DistanceL2();
	for(size_t i = 0;i<query_num;i++){
		vector<bool> isExist(base_num,false);  //验证该点是否已经存在 
		pool.clear();
		unsigned start_index = rand()%base_num;
		double dist = (double)distance_->compare(base_data[start_index].data(),query_data[i].data(),dim);
		Candidate_List point = {start_index,dist,false};
		pool.push_back(point);
		isExist[start_index] = true;
		unsigned index = first_unchecked(pool);
		while(index < L){
			unsigned new_index = pool[index].index;
			pool[index].isChecked = true;
			pool.reserve(L+k);
			for(size_t j = 0;j<k;j++){
				if(isExist[knn_graph[new_index][j]] == false){
					double dist = (double)distance_->compare(base_data[knn_graph[new_index][j]].data(),query_data[i].data(),dim);
					//cout<<dist<<endl;
					Candidate_List new_point = {knn_graph[new_index][j],dist,false};
					pool.push_back(new_point);  //添加新邻居点 
					isExist[knn_graph[new_index][j]] = true;
				}	
			}
			sort(pool.begin(),pool.end(),LessSort);  //按距离升序排序
			if(pool.size()>L)
				pool.resize(L);
			index = first_unchecked(pool);
		}
		//cout<<i<<endl;
		res[i].resize(K);
		for(size_t j=0;j<K;j++){
			res[i][j] = pool[j].index;
		}
	}
}
 

unsigned KNNSearch::first_unchecked(vector<Candidate_List> pool){
	unsigned i;
	for(i=0;i<pool.size();i++){
		if(pool[i].isChecked == false)
			return i;
	}
	return i;
}


void KNNSearch::Precision(){
	unsigned count = 0;
	double precision = 0;
	//unsigned res_num = res.size();
	unsigned res_num = QUERY_NUM;
	for(size_t i=0;i<res_num;i++){
		count = 0;
		for(size_t j=0;j<K;j++){
			if(res[i][j] == accurate_res[i][j]){
				count++;
			}
			else{
				for(size_t z=0;z<K;z++){
					if(res[i][j] == accurate_res[i][z]){
						count++;
						break;
					}
				}
			}
		}
		precision += (double)count/K;
		//cout<<(double)count/K<<endl; //输出每一次的精度 
	}
	precision = (double)precision/res_num;
	cout<<"precision:"<<precision<<endl;
}


int main(int argc, char** argv) {
	if (argc != 7 && argc != 8) {
    	cout<<argv[0]<<argc 
    	<<" data_file query_file nn_graph_path search_L search_K result_path (accurate_file)"<<endl;
    	exit(-1);
  	} 
  	
	KNNSearch search;
	//建议文件名称
	//数据集：   file/sift_base.fvecs 和  file/gist_base.fvecs
	//查询文件： file/sift_query.fvecs 和 file/gist_query.fvecs 
  	//knn图文件： file/sift_100NN_100.graph 和 file/gist_100NN_100.graph 
  	//结果文件：  res/sift_res.ivecs 和 res/sift_1000_accurate_res.ivecs
  	
  	search.load_data(argv[1],search.get_base_data());
  	search.load_graph(argv[3]);
  	search.load_data(argv[2],search.get_query_data());
  	search.SetK((unsigned)atoi(argv[5])); //默认100 
  	search.SetL((unsigned)atoi(argv[4]));

 	if ((unsigned)atoi(argv[4]) < (unsigned)atoi(argv[5])) {
   		cout<<"search_L cannot be smaller than search_K!"<<endl;
    	exit(-1);
  	}
  
  	
  	//1、若要运行Serial_Scan算法，得到准确结果，请将下面一段解注释 
//  	auto s = chrono::high_resolution_clock::now(); //记录开始时间 
//  	search.Serial_Scan();
//  	auto e = chrono::high_resolution_clock::now(); //记录结束时间 
//  	chrono::duration<double> diff = e - s;
//  	cout<<"search time: "<<diff.count()<<"s"<<endl;
//  	search.save_result(argv[6],search.get_accurate_res());
//  	//search.load_result(argv[6],search.get_accurate_res(),1); //查看准确结果 
  	
  	
  	//2、基于KNN图的搜素算法，运行下面一段 
  	auto s = chrono::high_resolution_clock::now(); //记录开始时间 
  	search.Search();
  	auto e = chrono::high_resolution_clock::now(); //记录结束时间 
  	chrono::duration<double> diff = e - s;
  	cout<<"search time: "<<diff.count()<<"s"<<endl;
  	search.save_result(argv[6],search.get_res());
  	//search.load_result(argv[6],search.get_res(),1); //查看近似搜索结果
  	search.load_result(argv[7],search.get_accurate_res(),2); //导入准确结果 
  	search.Precision();  //精度计算
	   
	//用于获得程序的峰值内存占用
	HANDLE handle = GetCurrentProcess();    
    PROCESS_MEMORY_COUNTERS pmc;    
    GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));
	cout<<"peak memory:"<<(double)pmc.PeakWorkingSetSize/1024/1024<<"MB";
  	return 0;
}

