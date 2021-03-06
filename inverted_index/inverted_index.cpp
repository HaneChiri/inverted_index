
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <string>//cout重载的是string头文件内的string 类型，而不是cstring头文件里面的

#include <iostream>
#include <algorithm>//使用了sort函数
using namespace std;

const int MAX_FILENAME_LEN = 256;
struct index
{
	vector<string> dict;
	vector<int> posting;
}indexList;

string fileList[2] = 
{
	"doc1.txt",
	"doc2.txt",
};



int getDict(FILE* fp);
void indexMulFile(string fileList[], int n)
{
	for (int i = 0; i < n; i++)
	{
		FILE*fp = fopen(fileList[i].c_str(), "r");
		getDict(fp);

	}

}



int getDict(FILE* fp)
{
	char ch = ' ';
	string term;
	int num = 0;//词的数目
	while (!feof(fp))
	{//一次循环获取一个单词
		//找到第一个字母
		do
		{
			ch = fgetc(fp);
			if (feof(fp)) break;//防止空文件导致的无限循环
		} while (!isalpha(ch));
		//读取单词
		while (isalpha(ch))
		{
			term += ch;
			ch = fgetc(fp);
		}
		indexList.dict.push_back(term);//把单词加入词典
		term = "";
		num++;

	}
	sort(indexList.dict.begin(), indexList.dict.end());

	fclose(fp);
	return 0;
}

int main()
{
	indexMulFile(fileList, 6);

	//自动去重，后面需要统计，因此需要改为手动去重
	indexList.dict.erase(unique(indexList.dict.begin(), indexList.dict.end()), indexList.dict.end());
	//unique把重复元素放在前面并返回不重复序列的最后一个元素的下一个元素

	//输出字典
	for (int i = 0; i < indexList.dict.size(); i++)
	{
		cout << indexList.dict[i] << endl;
	}
	return 0;
}