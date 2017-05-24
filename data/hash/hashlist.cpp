//相当于把pair类型封装在了节点中
#include<iostream>
#include<limits.h>
#include<string>
#include<utility>
#include<hash>
using namespace std;

class hashlist;

struct node
{
	friend class hashlist;
	node* next = NULL;
	pair<int,string> pat;
	node(,const pair<int,string>& pat_):pat(pat_){};
};


class hashlist
{
	private:
		node** table;
		hash<int> hash;
		int size;
		int div;
	public:
		hashlist(int thediv);
		~hashlist();

		int size() {return size;};
		bool empty() {return size==0;};
		int search(const int& thekey) const;
		pair<int,string>* find(const int& thekey) const;
		void insert(const pair<int,string>* thepair);
		void erase(const int& thekey);
};

hashlist::hashlist(int thediv)
{
	div = thediv;
	size = 0;

	table = new node*[div];
	for(int i=0;i<div;i++)
		table[i] = NULL;
}

hashlist::~hashlist()
{
	for(int i=0;i<div;i++)
	{
		if(table[i]==NULL)
			continue;
		else
		{			
			while(table[i]!=NULL)
			{
				node* st = table[i]->next;
				delete table[i];
				table[i] = st;
			}
		}
	}
}

int hashlist::search(const int& thekey)
{
	int i = (int)hash(thekey) % div;
	int j=i;
	do
	{
		if(table[j]==NULL)
			return j;
		else
		{
			node* st = table[j];
			while(st->next!=NULL)
			{
				if(st->pat->first == thekey)
					return j;
				st = st->next;
			}
		}

		j = (j+1)%div;
	}
	while(j!=i);
	return j;
}

pair<int,string>* hashlist::find(const int& thekey) const
{
	int b = search(thekey);

	if(table[b] == NULL)	
		return NULL;
	else
	{
		node* st = table[b];
		while(st->next!=NULL)
		{
			if(st->pat->first==thekey)
				return st->pat;
			st = st->next;
		}
		return NULL;
	}
}

void hashlist::insert(const pair<int,string>& thepair)
{
	int b = search(thepair.first);
	
	if(table[b]==NULL)
	{
		table[b] = new node(thepair);
		size++;
	}
	else
	{
		node* st = table[b];
		while(st!=NULL)
			st = st->next;
		st->next = new node(thepair);
		size++;
	}
}

void hashlist::erase(const int& thekey)
{
	int b = search(thekey);

	if(table[b]==NULL)
		return;
	else
	{
		node* st = table[b];
		while(st!=NULL)
		{
			if(st->next->pat->frist == thekey && st->next->next!=NULL)
			{
				node* at = st->next;
				st->next = at->next;
				delete at;
				size--;
			}
			if(st->next->pat->first==thekey && st->next->next==NULL)
			{
				node* at = st->next;
				st->next = NULL;
				delete at;
				size--;
			}
			st = st->next;
		}
	}
}
	







































