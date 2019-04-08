#include "CIndex.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;



//@name <CIndex::CIndex>
//@brief <��ʼ���ĵ���>
//@param <string p_collection[]:�ĵ��ļ�������><int n:���鳤��>
CIndex::CIndex(string p_collection[], int n)
{
	
	Document nextDoc;
	for (int i = 0; i < n; i++)
	{
		nextDoc.docName = p_collection[i];
		nextDoc.docID = i+1;//��Ŵ�1��ʼ
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
		cerr << "������Ϊ��" << endl;
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
//@brief <������ƪ�ĵ�>
//@param <FILE * fp:�Ѵ򿪵��ļ�ָ��><int docID:��������ļ��ı��>
//@return <ɨ�赽�Ĵ�������>
int CIndex::indexDocument(FILE * fp, int docID)
{
	
	char ch;//ɨ���õı���
	IndexItem indexItem;//����õı���
	int num = 0;//ɨ�赽�Ĵ�������

	while (!feof(fp))
	{//һ��ѭ����ȡһ������
		//�ҵ���һ����ĸ
		do
		{
			ch = fgetc(fp);
			if (feof(fp)) break;//��ֹ���ļ����µ�����ѭ��
		} while (!isalpha(ch));
		if (feof(fp)) break;//��ֹ���ļ�����Ŀ��ж��������ַ���
		//��ȡ���ʣ��������ֵ
		while (isalpha(ch))
		{
			indexItem.term += ch;
			ch = fgetc(fp);
		}
		
		indexItem.frequence = 1;
		indexItem.posting.push_back(docID);//�����ļ����ĵ�ID����posting
		//�����������ʵ�
		indexList.push_back(indexItem);
		num++;
		//��������׼����һ��
		indexItem.term="";
		indexItem.posting.clear();

	}

	return num;
	
}
//@name <CIndex::indexCollection>
//@brief <�����ĵ���>
//@return <��������Ŀ>
int CIndex::indexCollection()
{
	if (collection.empty())
	{
		cerr << "�ĵ���Ϊ��" << endl;
		return -1;
	}

	int num = 0;
	//�򿪶�Ӧ���ļ�������
	for (int i = 0; i < collection.size(); i++)
	{
		//���ļ�
		FILE* fp = fopen(collection[i].docName.c_str(), "r");
		//������ƪ�ĵ�
		num+=indexDocument(fp, collection[i].docID);
		//�ر��ļ�
		fclose(fp);
	}
	return num;
}
bool cmp(IndexItem a, IndexItem b)
{
	return a.term<b.term;//����մ�С��������
}
int CIndex::sortIndex()
{
	sort(indexList.begin(), indexList.end(), cmp);
	return 0;
}
//@name <CIndex::mergeIndex>
//@brief <������ϲ�ͬ����>

int CIndex::mergeSimilarTerms()
{
	if (indexList.empty())
	{
		cerr << "������Ϊ��" << endl;
		return -1;
	}
	IndexItem item1,item2;
	
	sortIndex();
	vector<IndexItem>::iterator it_cur=indexList.begin();//����������
	vector<IndexItem>::iterator it_next = it_cur + 1;
	vector<int> temp;
	vector<int>::iterator p1, p2;//���ںϲ�posting�ĵ�����
	while (it_cur != indexList.end())
	{
		if(it_cur+1!=indexList.end()) it_next = it_cur + 1;
		else break;
		while((*it_cur).term == (*it_next).term)
		{//���ѭ���ڴ���������뵱ǰ�����ظ��Ĵ���
			
			//�����ߵ�posting����
			sort((*it_cur).posting.begin(), (*it_cur).posting.end());
			sort((*it_next).posting.begin(), (*it_next).posting.end());
			//����ϲ����ߵ�posting
			p1 = (*it_cur).posting.begin();
			p2 = (*it_next).posting.begin();
			while (p1 != (*it_cur).posting.end() && p2 != (*it_next).posting.end())
			{
				if ((*p1) < (*p2))//������м����С��Ԫ��
				{
					temp.push_back(*p1);
					//���while���������ظ���Ԫ��
					
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
					//������ͬ�������������ƣ���������ظ�
					p1++;
					p2++;
				}
			}
			while(p1 != (*it_cur).posting.end())//�����1û�кϲ����򽫴�1���沿��ֱ�Ӹ���
			{
				temp.push_back(*p1);
				p1++;
			}
			while(p2 != (*it_next).posting.end())
			{
				temp.push_back(*p2);
				p2++;
			}
			//ɾ��������ظ�����
			temp.erase(unique(temp.begin(), temp.end()), temp.end());

			(*it_cur).frequence++;//��Ƶ����
			(*it_cur).posting.assign(temp.begin(), temp.end());//���������
			indexList.erase(it_next);//ɾ���ظ���
			temp.clear();
			if (it_cur + 1 != indexList.end()) it_next = it_cur + 1;
			else break;
		}

		it_cur++;
	}
	return 0;
}


int CIndex::findTerm(string term)
{//δ�޸������ã���ע��
	mergeSimilarTerms();
	iter_temp=indexList.begin();
	while (iter_temp != indexList.end())
	{
		//��todo�����Ż�Ϊ���ֲ���
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
		//��todo�����Ż�Ϊ���ֲ���
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
		//��todo�����Ż�Ϊ���ֲ���
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
