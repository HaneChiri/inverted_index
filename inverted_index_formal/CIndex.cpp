#include "CIndex.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;



//@name <CIndex::CIndex>
//@brief <初始化文档集>
//@param <string p_collection[]:文档文件名数组><int n:数组长度>
CIndex::CIndex(string p_collection[], int n)
{
	
	Document nextDoc;
	for (int i = 0; i < n; i++)
	{
		nextDoc.docName = p_collection[i];
		nextDoc.docID = i+1;//编号从1开始
		collection.push_back(nextDoc);
	}
	
}

void CIndex::showCollection()
{
	for (int i = 0; i < collection.size(); i++)
	{
		cout << collection[i].docID <<"\t"<< collection[i].docName << endl;
	}
}

void CIndex::showIndexList()
{
	if (indexList.empty())
	{
		cerr << "索引表为空" << endl;
		return;
	}
	cout << setw(20) << "term"<<setw(15)<<"frequence"<<setw(15)<<"posting" << endl;
	for (int i = 0; i < indexList.size(); i++)
	{
		cout << setw(20) << indexList[i].term 
			<< setw(15) << indexList[i].frequence<<"\t->\t";
		for (int j = 0; j < indexList[i].posting.size(); j++)
		{
			cout << indexList[i].posting[j] << "\t";
		}
		cout << endl;
	}
}
//@name <CIndex::indexDocument>
//@brief <索引单篇文档>
//@param <FILE * fp:已打开的文件指针><int docID:分配给此文件的编号>
//@return <扫描到的词项数量>
int CIndex::indexDocument(FILE * fp, int docID)
{
	
	char ch;//扫描用的变量
	IndexItem indexItem;//打包用的变量
	int num = 0;//扫描到的词项数量

	while (!feof(fp))
	{//一次循环获取一个单词
		//找到第一个字母
		do
		{
			ch = fgetc(fp);
			if (feof(fp)) break;//防止空文件导致的无限循环
		} while (!isalpha(ch));
		if (feof(fp)) break;//防止因文件后面的空行而索引空字符串
		//读取单词，给索引项赋值
		while (isalpha(ch))
		{
			indexItem.term += ch;
			ch = fgetc(fp);
		}
		
		indexItem.frequence = 1;
		indexItem.posting.push_back(docID);//将本文件的文档ID加入posting
		//把索引项加入词典
		indexList.push_back(indexItem);
		num++;
		//清空索引项，准备下一次
		indexItem.term="";
		indexItem.posting.clear();

	}

	return num;
	
}
//@name <CIndex::indexCollection>
//@brief <索引文档集>
//@return <词项总数目>
int CIndex::indexCollection()
{
	if (collection.empty())
	{
		cerr << "文档集为空" << endl;
		return -1;
	}

	int num = 0;
	//打开对应的文件并索引
	for (int i = 0; i < collection.size(); i++)
	{
		//打开文件
		FILE* fp = fopen(collection[i].docName.c_str(), "r");
		//索引单篇文档
		num+=indexDocument(fp, collection[i].docID);
		//关闭文件
		fclose(fp);
	}
	return num;
}
bool cmp(IndexItem a, IndexItem b)
{
	return a.term<b.term;//词项按照从小到大排序
}
int CIndex::sortIndex()
{
	sort(indexList.begin(), indexList.end(), cmp);
	return 0;
}
//@name <CIndex::mergeIndex>
//@brief <索引表合并同类项>

int CIndex::mergeSimilarTerms()
{
	if (indexList.empty())
	{
		cerr << "索引表为空" << endl;
		return -1;
	}
	IndexItem item1,item2;
	
	sortIndex();
	vector<IndexItem>::iterator it_cur=indexList.begin();//创建迭代器
	vector<IndexItem>::iterator it_next = it_cur + 1;
	vector<int> temp;
	vector<int>::iterator p1, p2;//用于合并posting的迭代器
	while (it_cur != indexList.end())
	{
		if(it_cur+1!=indexList.end()) it_next = it_cur + 1;
		else break;
		while((*it_cur).term == (*it_next).term)
		{//这个循环内处理掉所有与当前词项重复的词项
			
			//将二者的posting排序
			sort((*it_cur).posting.begin(), (*it_cur).posting.end());
			sort((*it_next).posting.begin(), (*it_next).posting.end());
			//有序合并两者的posting
			p1 = (*it_cur).posting.begin();
			p2 = (*it_next).posting.begin();
			while (p1 != (*it_cur).posting.end() && p2 != (*it_next).posting.end())
			{
				if ((*p1) < (*p2))//结果集中加入较小的元素
				{
					temp.push_back(*p1);
					//这个while用于跳过重复的元素
					
					p1++;
				}
				else if((*p1) > (*p2))
				{
					temp.push_back(*p2);
					
					p2++;
				}
				else
				{
					temp.push_back(*p1);
					//遇到相同的则两个都后移，避免出现重复
					p1++;
					p2++;
				}
			}
			while(p1 != (*it_cur).posting.end())//如果串1没有合并完则将串1后面部分直接复制
			{
				temp.push_back(*p1);
				p1++;
			}
			while(p2 != (*it_next).posting.end())
			{
				temp.push_back(*p2);
				p2++;
			}
			//删除结果集重复部分
			temp.erase(unique(temp.begin(), temp.end()), temp.end());

			(*it_cur).frequence++;//词频增加
			(*it_cur).posting.assign(temp.begin(), temp.end());//将结果复制
			indexList.erase(it_next);//删除重复项
			temp.clear();
			if (it_cur + 1 != indexList.end()) it_next = it_cur + 1;
			else break;
		}

		it_cur++;
	}
	return 0;
}


int CIndex::findTerm(string term)
{//未修复不可用，请注意
	mergeSimilarTerms();
	iter_temp=indexList.begin();
	while (iter_temp != indexList.end())
	{
		//【todo】可优化为二分查找
		if ((*iter_temp).term == term)
		{
			return 0;
		}
		
		iter_temp++;
	}

	return -1;
}



vector<int> CIndex::bool_and(string term1, string term2)
{
	mergeSimilarTerms();
	vector<IndexItem>::iterator it_term1;
	vector<IndexItem>::iterator it_term2;
	vector<IndexItem>::iterator it = indexList.begin();
	while (it != indexList.end())
	{
		//【todo】可优化为二分查找
		if ((*it).term == term1)
		{
			it_term1 = it;
			break;
		}
		it++;
	}
	it = indexList.begin();
	while (it != indexList.end())
	{
		//【todo】可优化为二分查找
		if ((*it).term == term2)
		{
			it_term2 = it;
			break;
		}
		it++;
	}



	vector<int>::iterator p1 = (*it_term1).posting.begin();
	vector<int>::iterator p2 = (*it_term2).posting.begin();
	vector<int> res;

	while (p1 != (*it_term1).posting.end() && p2 != (*it_term2).posting.end())
	{
		if ((*p1) == (*p2))
		{
			res.push_back((*p1));
			p1++;
			p2++;
		}
		else if((*p1)<(*p2))
		{
			p1++;
		}
		else
		{
			p2++;
		}
	}


	return res;
}


CIndex::~CIndex()
{
}
