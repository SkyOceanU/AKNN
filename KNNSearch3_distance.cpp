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
  	in.read((char*)&dim, 4);	//��ȡ����ά��
  	in.seekg(0, ios::end);	
  	ios::pos_type ss = in.tellg();	//��ȡ�ļ���С�������ֽڣ�
  	size_t fsize = (size_t)ss;
  	unsigned num = (unsigned)(fsize / (dim + 1) / 4);	//�������ݸ���
  	data.resize(num);
  	data.reserve(num);

  	in.seekg(0, ios::beg);
 	for(size_t i = 0; i < num; i++){
    	in.seekg(4, ios::cur);
    	data[i].resize(dim);
    	in.read((char*)data[i].data(), dim * sizeof(float));	//��ȡ���ݵ�data��
  	}
	in.close();
}


void KNNSearch::load_graph(char* filename){
  	ifstream in(filename,ios::binary);
  	in.read((char *)&k, sizeof(unsigned));
 	in.seekg(0, ios::end);
  	ios::pos_type ss = in.tellg();
  	size_t fsize = (size_t)ss;
  	size_t num = (unsigned)(fsize / (k + 1) / 4);  //�������ݸ���
  	in.seekg(0, ios::beg);

  	knn_graph.resize(num);
  	knn_graph.reserve(num);
  	unsigned kk = (k + 3) / 4 * 4;
  	for(size_t i = 0; i < num; i++){
    	in.seekg(4, ios::cur);
    	knn_graph[i].resize(k);
    	knn_graph[i].reserve(kk);
    	in.read((char *)knn_graph[i].data(), k * sizeof(unsigned));  //��ȡ���� 
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
  	size_t num = (unsigned)(fsize / (K + 1) / 4);  //�������ݸ���
  	in.seekg(0, ios::beg);

  	result.resize(num);
  	for(size_t i = 0; i < num; i++){
    	in.seekg(4, ios::cur);
    	result[i].resize(K);
    	in.read((char *)result[i].data(), K * sizeof(unsigned));  //��ȡ���� 
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
	vector<Candidate_List> pool(L);  //�����ѡ��
	srand((int)time(0));  //������������� 
	//size_t query_num = query_data.size();
	size_t query_num = QUERY_NUM;
	size_t base_num = base_data.size();
	res.resize(query_num);
	DistanceL2* distance_ = new DistanceL2();
	for(size_t i = 0;i<query_num;i++){
		vector<bool> isExist(base_num,false);  //��֤�õ��Ƿ��Ѿ����� 
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
					pool.push_back(new_point);  //������ھӵ� 
					isExist[knn_graph[new_index][j]] = true;
				}	
			}
			sort(pool.begin(),pool.end(),LessSort);  //��������������
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
		//cout<<(double)count/K<<endl; //���ÿһ�εľ��� 
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
	//�����ļ�����
	//���ݼ���   file/sift_base.fvecs ��  file/gist_base.fvecs
	//��ѯ�ļ��� file/sift_query.fvecs �� file/gist_query.fvecs 
  	//knnͼ�ļ��� file/sift_100NN_100.graph �� file/gist_100NN_100.graph 
  	//����ļ���  res/sift_res.ivecs �� res/sift_1000_accurate_res.ivecs
  	
  	search.load_data(argv[1],search.get_base_data());
  	search.load_graph(argv[3]);
  	search.load_data(argv[2],search.get_query_data());
  	search.SetK((unsigned)atoi(argv[5])); //Ĭ��100 
  	search.SetL((unsigned)atoi(argv[4]));

 	if ((unsigned)atoi(argv[4]) < (unsigned)atoi(argv[5])) {
   		cout<<"search_L cannot be smaller than search_K!"<<endl;
    	exit(-1);
  	}
  
  	
  	//1����Ҫ����Serial_Scan�㷨���õ�׼ȷ������뽫����һ�ν�ע�� 
//  	auto s = chrono::high_resolution_clock::now(); //��¼��ʼʱ�� 
//  	search.Serial_Scan();
//  	auto e = chrono::high_resolution_clock::now(); //��¼����ʱ�� 
//  	chrono::duration<double> diff = e - s;
//  	cout<<"search time: "<<diff.count()<<"s"<<endl;
//  	search.save_result(argv[6],search.get_accurate_res());
//  	//search.load_result(argv[6],search.get_accurate_res(),1); //�鿴׼ȷ��� 
  	
  	
  	//2������KNNͼ�������㷨����������һ�� 
  	auto s = chrono::high_resolution_clock::now(); //��¼��ʼʱ�� 
  	search.Search();
  	auto e = chrono::high_resolution_clock::now(); //��¼����ʱ�� 
  	chrono::duration<double> diff = e - s;
  	cout<<"search time: "<<diff.count()<<"s"<<endl;
  	search.save_result(argv[6],search.get_res());
  	//search.load_result(argv[6],search.get_res(),1); //�鿴�����������
  	search.load_result(argv[7],search.get_accurate_res(),2); //����׼ȷ��� 
  	search.Precision();  //���ȼ���
	   
	//���ڻ�ó���ķ�ֵ�ڴ�ռ��
	HANDLE handle = GetCurrentProcess();    
    PROCESS_MEMORY_COUNTERS pmc;    
    GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));
	cout<<"peak memory:"<<(double)pmc.PeakWorkingSetSize/1024/1024<<"MB";
  	return 0;
}

