#pragma once
#include <string>
#include <vector>
using namespace std;
struct Document
{
	string docName;//文档名
	int docID;//文档id
};
struct IndexItem
{
	string term;//词项
	int frequence;//词频
	vector<int> posting;
};

class CIndex
{
	vector<IndexItem> indexList;//索引表
	vector<Document> collection;//文档集
public:
	//利用文件名数组初始化文档集
	CIndex(string p_collection[], int n);
	//显示文档集内所有文档的文件名
	void showCollection();
	//显示当前倒排索引表
	void showIndexList();
	//索引单篇文档
	int indexDocument(FILE*fp, int docID);
	//索引文档集
	int indexCollection();
	//排序索引表
	int sortIndex();
	//索引表合并同类项
	int mergeSimilarTerms();
	//查找某个词项，不可用
	vector<IndexItem>::iterator* findTerm(string term);
	//布尔查询and
	vector<int> bool_and(string term1, string term2);

	//多个词项and
	~CIndex();
};

