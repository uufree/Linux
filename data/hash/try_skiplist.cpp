#include<iostream>
#include<string>
#include<utility>
#include<cstdlib>
#include<cmath>
using namespace std;

class skiplist;

struct node
{
	firend class skiplist;
	pair<int,string> element;
	node** next;

	node(const pair<int,string>& thepair,int size):element(thepair){next = new node*[size];};

};

class skiplist
{
	private:
		float cutoff;
		int levels;
		int size;
		int maxlevel;
		int tailkey;

		node* head;
		node* tail;
		node** last;

	public:
		skiplist(int largekey,int maxpairs,float prob);
		~skipnode();

		int size(){return size;};
		bool empty(){return size==0;};
		pair<int,string>* find(const int& thekey) const;
		int level() const;
		node* search(const int& thekey) const;
		void insert(const pair<int,string>& thepair);
		void erase(const int& thekey);
};

skiplist::skiplist(int largekey,int maxpairs,float prob)
{
	cutoff = prob *	RAND_MAX;
	maxlevel = (int)ceil(logf((float)maxpairs)/logf(1/prob)) - 1;
	levels = 0;
	size = 0;
	tailkey = largekey;

	pair<int,string> thetail;
	thetail.first = tailkey;
	head = new node(thetail,maxlelevel+1);
	tail = new node(thetail,0);
	last = new node*(maxlevel+1);

	for(int i=0;i<=maxlevel;i++)
		head->next[i] = tail;
}

pair<int,string>* skiplist::find(const int& thekey) const
{
	if(thekey>=tailkey)
		return NULL;

	node* newnode = head;
	for(int i=maxlevel;i>=0;i--)
		while(newnode->next[i]->element.first < thekey)
			newnode = newnode->next[i];

	if(newnode->next[0]->element.first == thekey)
		return &newnode->next[0]->element;

	return NULL;
}

int skiplist::level() const
{
	int level = 0;
	while(rand()<cutoff)
		level++;
	return (level<maxlevel)?level:maxlevel;
}

node* skiplist::search(const int& thekey) const
{
	node* st = head;
	for(int i=maxlevel;i>=0;i--)
	{
		while(st->next[i]->element.first < thekey)
			st = st->next[i];

		last[i] = st;
	}
	return st->next[0];
}

void skiplist::insert(const pair<int,string>& thepair)
{
	node* thenode = search(thepair.first);
	if(thenoode->element.first == thepair.first)
	{
		thenode->element->second = thepair.second;
		return;
	}

	int thelevel = level();
	if(thelevel > levels)
	{
		thelevel = ++levels;
		last[thelevel] = head;
	}

	node* newnode = new node(thepair,thelevel)
	for(int i=0;i<thelevel;i++)
	{
		newnode->next[i] = last->next[i];
		last[i]->next[i] = newnode;
	}
	size++;
}

void skiplist::erase(const int& thekey)
{
	if(thekey>tailkey)
		return;

	node* st = search(thekey);
	if(st->element.first != thekey)	
		return;

	for(int i=0;i<levels && last[i]->next==st;i++)
		last[i]->next[i] = st->next[i];
	
	while(levels>0 && head->next[levels]==tail)
		levels--;

	delete st;
	size--;
}




























