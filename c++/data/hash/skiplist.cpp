//跳表规定链表必须已经排好序
#include<iostream>
#include<string>
#include<utility>
#include<cmath>
#include<cstdlib>
using namespace std;


struct node
{
	friend class skiplist;
	typedef pair<int,string> pair;
	pair element;
	node** next;
	
	node(const pair& st,int size):element(st) {next = new node*[size];};
};

class skiplist
{
	private:
		float cutoff;//用来确定层数
		int levels;//当前最大的非空链表
		int size;//数对个数
		int maxlevel;//允许的最大链表层数
		int tailkey;//最大的关键字

		node* head;//头节点指针
		node* tail;//尾节点指针
		node** last;//last[i]表示i层的最后节点

	public:
		skiplist(int largekey,int maxpairs,float prob);
		~skiplist();
		
		int size() {return size;};
		bool empty() {return size == 0;};
		pair<int,string>* find(const int& thekey) const;//根据索引找到数据
		int level() const;//级的分配方法
		node* search(const int& thekey) const;//索引并把在每一级遇到的最后一个节点指针存储起来,为插入时的级数做准备
		void insert(const pair<int,string>& thepair);//跳表插入
		void erase(const int& thekey);//跳表删除
}


skiplist::skiplist(int largekey,int maxpairs,float prob)
{//索引最大数字；剩下的两个用来确定级数
	cutoff = prob * RAND_MAX;//用来确定层数
	maxlevel = (int)ceil(logf((float)maxpairs)/logf(1/prob)) - 1;
	levels = 0;//初始层数
	size = 0;//初始数对个数
	tailkey = largekey;//最大的索引数字

	pair<int,string> thetail;//新建一个pair对象
	thetail.first = tailkey;//给该对象赋值
	head = new node(thetail,maxlevel+1);//头节点拥有所有的层数
	tail = new node(thetail,0);//尾节点只有一个层数
	last = new node*(maxlevel+1);//在插入时完善级数的安排

	for(int i=0;i<=maxlevel;i++)
		head->next[i] = tail;//头节点的每一层都指向尾节点
};

pair<int,string>* skiplist::find(const int& thekey) const
{//根据索引找数据
	if(thekey >= tailkey)//如果索引大于最大的数字直接返回
		return NULL;

	node* beforenode = head;//从最开始查找
	for(int i=levels;i>=0;i--)//从最高层索引，逐层下降
		while(beforenode->next[i]->element.first < thekey)
			beforenode = beforenode->next[i];//逐步逼近

	if(beforenode->next[0]->element.first == thekey)
		return &beforenode->next[0]->element;
	
	return NULL;
}

int skiplist::level() const
{
	int level=0;
	while(rand()<cutoff)//根据随机值确定级数
		level++;
	return (level<=maxlevel) ? level:maxlevel;
}

node* skiplist::serach(const int& thekey) const
{
	node* beforenode = head;//以头节点开始
	for(int i=levels;i>=0;i--)//逐层下降
	{
		while(beforenode->next[i]->element.first<thekey)
			beforenode = beforenode->next[i];//逐步逼近
	
		last[i] = beforenode;//查找到最近的左值，保存
	}

	return beforenode->next[0];//返回查找值
}

void skiplist::insert(const pair<int,string>& thepair)
{
	node* thenode = search(thepair.first);
	if(thenode->element.first == thepair.first)
	{//如果关键字已经存在则直接返回	
		thenode->element.second = thepair.second;
		return;
	}

	int thelevel = level();//确定将要插入的节点的级数
	if(thelevel>levels)//确定级数小于等于当前的级数+1
	{
		thelevel = ++levels;
		last[thelevel] = head;//该节点的最大级数为头节点
	}

	node* newnode = new node(thepair,thelevel+1);//确定插入对象
	for(int i=0;i<thelevel;i++)//每一层的节点插入
	{
		newnode->next[i] = last->next[i];
		last[i]->next[i] = newnode;
	}

	size++;
}

void skiplist::erase(const int& thekey)
{
	if(thekey > tailkey)//如果关键字大于，直接返回
		return;

	node* thenode = search(thekey);//寻找删除对象的位置
	if(thenode->element.first!=thekey)//如果索引不匹配，直接返回
		return;

	for(int i=0;i<levels && last[i]->next[i]==thenode;i++)//根据last删
		last[i]->next[i] = thenode->next[i];

	while(levels>0 && head->next[levels]==tail)//表示实时的层数
		levels--;//根据下一个节点是否指向尾节点忽略当前层数
	delete thenode;
	size--;
}

