//双向链表可以避免无谓的循环 
#include<iostream>
using namespace std;

struct node
{
	friend class list;
	int ans;
	node* next;
	node* front;
	node(int ans_,node* next_=NULL,node* front_=NULL):ans(ans_),next(next_),front(front_){};
};

class list
{
private:
	node* head;
	int listsize;
public:
	//三五法则 
	list(int size=0);
	list(const list & st);
	~list();
	//ADT
	bool empty(){return listsize == 0;};
	int size(){return listsize;}
	int& get(int index) const;
	int indexof(const int& element);
	void erase(int index = 1);
	void insert(int index,const int& key);
	void output();
	//个性化设计
	void clear();
	void push_back(const int& key); 
	void setsize(int size);
	void remove(int left,int right);
	int& operator[](int index);
	friend bool operator==(list& st,list& at);
	friend bool operator!=(list& st,list& at);
	friend bool operator<(list& st,list& at);
	void leftshift(int it);
//	void reverse(); //不适合循环链表 
};

list::list(int size)
{
	listsize = size;
	head = new node(0);
}

list::list(const list& st)
{
	listsize = st.listsize;
	node* first = st.head;
	head = new node(first->ans);
	node* head_0 = head;
	first = firts->next;
	while(first!=NULL)
	{
		head_0->next = new node(frist->ans);
		head_0->next->front = head_0;
		first = first->next;
		head_0 = head_0->next;
	}
	head_0->next = head->next;
	head->next->front = head_0;
}

list::~list()
{
	for(int i=0;i<listsize;i++)
		erase();
	delete head;
}

void list::insert(int index,const int& key)
{
	if(index<listsize && index==0)//一旦进行头插入，则头节点的front发生改变 
	{
		node* st = head;
		node* at = head;
		for(int i=0;i<listsize;i++)
			at = at->next;
		st->next = new node(100,st->next,at);
 		head->next->next->front = st->next;
		at->next = head->next;
		listsize++;
	} 
	else if(index<listsize && index>0)
	{
		node* st = head;
		for(int i=0;i<index;i++)
			st = st->next;
		st->next = new node(key,st->next,st);
		st->next->next->front = st->next;
		listsize++;
	}
	else if(index==listsize)
	{
		node* st = head;
		for(int i=0;i<listsize;i++)
			st = st->next;
		st->next = new node(key,head->next,st);
		head->next->front = st->next;
		listsize++;
	}
	else
		cout << "Wrong!" << endl;
}

void list::erase(int index)
{
	if(index>0)
	{
		if(index<listsize && index==1)
		{
			node* st = head;
			node* at = head;
			for(int i=1;i<listsize;i++)
				at = at->next;
			node* ad = st->next;
			st->next = st->next->next;
			st->next->front = at;
			at->next = st->next;
			delete ad;
			listsize--;	
		}
		else if(index<=listsize && index!=1)
		{
			node* at = head;
			for(int i=1;i<index;i++)
				at = at->next;
			node* st = at->next;	
			at->next = at->next->next;
			at->next->front = at;
			delete st;	
			listsize--;
		}
		else
			cout << "Wrong!" << endl;
	}
	else
		cout << "Wrong!" << endl;
}































































































































