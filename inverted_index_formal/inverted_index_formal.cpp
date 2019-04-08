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
	vector<int> a = in.bool_and("w", "well");
	if (a.empty())
	{
		cout << "空集" << endl;
	}
	else
	{
		for (int i = 0; i < a.size(); i++)
		{
			cout << a.at(i);
		}
	}
	return 0;
}