#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <windows.h>    
#include <psapi.h>
#include <functional>
#pragma  comment(lib,"psapi.lib")

using namespace std;

struct Candidate_Pool{
	unsigned index;  //索引
	double dis;  //离查询点的距离 
};

struct Candidate_List{
	unsigned index;  //索引
	double dis;  //离查询点的距离
	bool isChecked;  //判断是否已经检查过 
};


class KNNSearch{
	
protected:
	vector<vector<float > > base_data;    //基础数据
	vector<vector<unsigned > > knn_graph; //构造好的kNN图
	vector<vector<float > > query_data;  //查询数据
	vector<vector<unsigned > > res;     //最终结果 
	vector<vector<unsigned > > accurate_res;  //最终准确结果 
	unsigned K;   //待查询近邻数K（默认100）	
	unsigned L;   //Candidate Pool大小L
	unsigned dim; //数据维度
	unsigned k;  //kNN图的k大小 
	
public:
	void load_data(char* filename,vector<vector<float > >&);  //导入数据集
	void load_graph(char* filename);  //导入kNN图
	void load_result(char* filename,vector<vector<unsigned > >& result,int choice);  //导入计算结果 
	void save_result(char* filename,vector<vector<unsigned > >& result); //保存计算结果 
	void Search();  //搜索过程
	void Serial_Scan();  //串行扫描，获得精确邻近结果 
	double distance(vector<float> point1,vector<float> point2); //计算欧氏距离
	void Precision();  // 计算平均精度； 
	unsigned first_unchecked(vector<Candidate_List> pool);  //返回第一个未检查的点的索引
	//static bool LessSort(Candidate_Pool a,Candidate_Pool b);   //用于对候选区排序
	void SetK(unsigned KK){K = KK;}  //设置K
	void SetL(unsigned LL){L = LL;}  //设置L 
	vector<vector<float > >& get_base_data(){return base_data;}  //返回基础数据引用 
	vector<vector<float > >& get_query_data(){return query_data;}  //返回查询数据引用 
	vector<vector<unsigned > >& get_res(){return res;}  //返回最终结果引用 
	vector<vector<unsigned > >& get_accurate_res(){return accurate_res;}  //返回最终准确结果引用 
	void create_graph(char* filename,int num); //主要用于生成knn图（小于100) 
};



