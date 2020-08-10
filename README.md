AKNN
===
## 1、项目介绍
  AKNN主要针对近似最近邻搜索问题，是基于给定的KNN图的相关搜索算法
## 2、环境部署
* C++语言
* g++4.9.2编译

## 3、文件说明
### 文件夹
* data<br>
  主要包含表格data.xlsx，记录实验得到的所有数据
* file<br>
  主要包含搜索算法中需要的数据，如数据集的base data、knn图和query data<br>
    `注意：本实验主要是用SIFT1M和GIST1M数据集，可通过下面的相关链接进入下载，放至file文件夹中`<br>
    [SIFT1M和GIST1M](http://corpus-texmex.irisa.fr/)<br>
    [SIFT1M和GIST1M对应的100NN图](https://pan.baidu.com/s/1XGnJT62HeMMr5x0UIpytGg)  提取码：ey3t
* res<br>
  主要包含最终检索结果的导出文件，以及利用Serial Scan跑出的标准邻近点结果也存放在这里面
### 代码文件
* KNNSearch.cpp<br>
  * 主算法程序，可通过cmd运行
  * 同时包含了生成指定大小的knn图的函数
  * 注意需包含`KNNSearch.h`头文件
* KNNSearch1_x.cpp<br>
  * 1代表着选做题1
  * x代表着`extra`、`heap`和`insert`三种优化尝试
* KNNSearch3_distance.cpp<br>
  * 利用SSE/AVX指令集优化欧氏距离计算，本实验测试主要使用SSE2指令集
  * 注意需包含`distance.h`头文件
* test.cpp<br>
  * 包含一些数据文件导入、导出的函数
  * 可以用于方便地查看数据及结果文件等
## 4、运行
* 首先需要编译各代码文件，以KNNSearch.cpp为例，注意末尾需加上`-lpsapi`，用以链接获取内存占用操作<br>
```
g++ -std=c++11 -o KNNSearch KNNSearch.cpp -lpsapi
```
* 运行时需要按照指定数据格式进行参数输入<br>
```
KNNSearch.exe data_file query_file nn_graph_path search_L search_K result_path (accurate_file)
```
  * `data_file`代表导入的数据集的基础数据路径
  * `query_file`代表导入的数据集的查询数据路径
  * `nn_graph_path`代表导入的数据集的KNN图路径
  * `search_L`代表候选池的大小L，一般而言，当L越大时，精度会越高，但搜索速度会下降
  * `search_K`代表最后需返回的近邻点个数
  * `result_path`代表存储搜索结果的文件路径
  * `accurate_file`代表存储精确结果的文件路径，用以导入计算精度，为可选项<br>
  下面为一个具体使用的例子：
```
KNNSearch.exe file/sift_base.fvecs file/sift_query.fvecs file/sift_100NN_100.graph 100 100 res/sift_res_1000.ivecs res/sift_1000_accurate_res.ivecs
```
* 其他程序的运行格式也大致与上面相似
## 5、输出格式
  除了可以导出结果文件外，还可获得该搜索过程的时间、精度和峰值内存占用，具体格式示例如下：<br>
  ```
search time: 463.387s
precision:0.99721
peak memory:958.098MB
```
