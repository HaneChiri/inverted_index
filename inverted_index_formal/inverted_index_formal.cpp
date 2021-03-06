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
	in.showCollection();
	in.indexCollection();
	in.mergeSimilarTerms();
	in.showIndexList();
	string term1, term2;
	cout << "求交集:" << endl;
	cout << "输入词项1:" << endl;
	cin >> term1;

	cin.clear();
	cin.sync();

	cout << "输入词项2:" << endl;
	cin >> term2;

	vector<int> a = in.bool_and(term1, term2);
	cout <<"\""<<term1<<"\"和" << "\"" << term2 << "\"的交集:" << endl;
	if (a.empty())
	{
		cout << "空集" << endl;
	}
	else
	{
		
		for (int i = 0; i < a.size(); i++)
		{
			cout << a.at(i)<<" ";

			//
		}
	}
	return 0;
}