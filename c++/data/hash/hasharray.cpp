#include<iostream>
#include<utility>
#include<string>
#include<hash>
using namespace std;

class hasharray
{
	private:
		pair<int,string>** table;//散列表
		hash<int> hash;//采用STL的哈希函数
		int size;//已存在数字
		int div;//除数以及桶数量
	public:
		hasharray(int thediv);
		~hasharray();

		int size(){return size;};
		bool empty(){return size==0;};
		int search(const int& thekey) const;
		pair<int,string>* find(const int& thekey) const;
		void insert(const pair<int,string>& thepair);
		void erase(const int& thekey);
};

hasharray::hasharray(int thediv)
{
	div = thediv;//除数以及桶数量
	size = 0;

	table = new pair<int,string>*[div];
	for(int i=0;i<div;i++)
		table[i] = NULL;
}

int hasharray::search(const int& thekey) const//返回桶的序号
{
	int i = (int)hash(thekey) % div;//取得表首地址
	int j = i;
	do
	{
		if(table[j]==NULL || table[i]->first==thekey)
			return j;
		j = (j+1)%div;//取余操作，加1
	}while(j!=i);

	return j;
}

pair<int,string>* hasharray::find(const int& tnekey) const//寻找数对
{
	int b = search(thekey);//获得桶序号

	if(table[b]==NULL || table[b]->first!=thekey)
		return NULL;

	return table[b];
}

void hasharray::insert(const pair<int,string>& thepair)
{
	int b = seaech(thepair.first);/获得桶序号

	if(table[b]==NULL)//如果为空
	{
		table[b] = new pair<int,string>(the pair);
		size++;
	}
	else
	{
		if(table[b]->first == thepair.first)//如果索引相同，修改值
			table[b]->second = thepair.second;
		else
			return;//不休改，直接返回
	}
}

void hasharray::erase(const int& thekey)
{
	int b = search(thekey);
	
	if(table[b]==NULL)
		return;
	else
	{
		if(table[b]->first == thekey)
			delete table[b];
		else
			return;
	}
}




































