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
	unsigned index;  //����
	double dis;  //���ѯ��ľ��� 
};

struct Candidate_List{
	unsigned index;  //����
	double dis;  //���ѯ��ľ���
	bool isChecked;  //�ж��Ƿ��Ѿ����� 
};


class KNNSearch{
	
protected:
	vector<vector<float > > base_data;    //��������
	vector<vector<unsigned > > knn_graph; //����õ�kNNͼ
	vector<vector<float > > query_data;  //��ѯ����
	vector<vector<unsigned > > res;     //���ս�� 
	vector<vector<unsigned > > accurate_res;  //����׼ȷ��� 
	unsigned K;   //����ѯ������K��Ĭ��100��	
	unsigned L;   //Candidate Pool��СL
	unsigned dim; //����ά��
	unsigned k;  //kNNͼ��k��С 
	
public:
	void load_data(char* filename,vector<vector<float > >&);  //�������ݼ�
	void load_graph(char* filename);  //����kNNͼ
	void load_result(char* filename,vector<vector<unsigned > >& result,int choice);  //��������� 
	void save_result(char* filename,vector<vector<unsigned > >& result); //��������� 
	void Search();  //��������
	void Serial_Scan();  //����ɨ�裬��þ�ȷ�ڽ���� 
	double distance(vector<float> point1,vector<float> point2); //����ŷ�Ͼ���
	void Precision();  // ����ƽ�����ȣ� 
	unsigned first_unchecked(vector<Candidate_List> pool);  //���ص�һ��δ���ĵ������
	//static bool LessSort(Candidate_Pool a,Candidate_Pool b);   //���ڶԺ�ѡ������
	void SetK(unsigned KK){K = KK;}  //����K
	void SetL(unsigned LL){L = LL;}  //����L 
	vector<vector<float > >& get_base_data(){return base_data;}  //���ػ����������� 
	vector<vector<float > >& get_query_data(){return query_data;}  //���ز�ѯ�������� 
	vector<vector<unsigned > >& get_res(){return res;}  //�������ս������ 
	vector<vector<unsigned > >& get_accurate_res(){return accurate_res;}  //��������׼ȷ������� 
	void create_graph(char* filename,int num); //��Ҫ��������knnͼ��С��100) 
};



