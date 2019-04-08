#pragma once
#include <string>
#include <vector>
using namespace std;
struct Document
{
	string docName;//�ĵ���
	int docID;//�ĵ�id
};
struct IndexItem
{
	string term;//����
	int frequence;//��Ƶ
	vector<int> posting;
};

class CIndex
{
	vector<IndexItem> indexList;//������
	vector<Document> collection;//�ĵ���
public:
	//�����ļ��������ʼ���ĵ���
	CIndex(string p_collection[], int n);
	//��ʾ�ĵ����������ĵ����ļ���
	void showCollection();
	//��ʾ��ǰ����������
	void showIndexList();
	//������ƪ�ĵ�
	int indexDocument(FILE*fp, int docID);
	//�����ĵ���
	int indexCollection();
	//����������
	int sortIndex();
	//������ϲ�ͬ����
	int mergeSimilarTerms();
	//����ĳ�����������
	vector<IndexItem>::iterator* findTerm(string term);
	//������ѯand
	vector<int> bool_and(string term1, string term2);

	//�������and
	~CIndex();
};

