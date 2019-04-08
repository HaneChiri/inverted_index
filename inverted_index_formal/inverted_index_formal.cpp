#include <iostream>
#include <algorithm>
#include "CIndex.h"
#include <string>
using namespace std;
string fileList[6] =
{
	"doc1.txt",
	"doc2.txt",
	"doc3.txt",
	"doc4.txt",
	"doc5.txt",
	"doc6.txt"
};
int main()
{
	CIndex in(fileList,6);
	cout << "索引的文档：" << endl;
	in.showCollection();
	in.indexCollection();
	in.mergeSimilarTerms();
	in.showIndexList();
	cout << "请输入要查找的两个词项：" << endl;
	cout << "词项1：" << endl;
	string term1;
	cin >> term1;

	cin.clear();
	cin.sync();

	cout << "词项1：" << endl;
	string term2;
	cin >> term2;
	vector<int> a = in.bool_and(term1, term2);
	cout << "\"" << term1 << "\"和" << "\"" << term2 << "\"的posting交集：" << endl;
	if (a.empty())
	{
		cout << "空集" << endl;
	}
	else
	{
		for (int i = 0; i < a.size(); i++)
		{
			cout << a.at(i)<<"\t";
		}
	}
	return 0;
}