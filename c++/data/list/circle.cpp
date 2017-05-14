#include<iostream>
using namespace std;

class list;

struct node
{
	friend class list;
	int ans;
	node* next;
	node(int ans_,node* next_=NULL):ans(ans_),next(next_){};
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
	void insert(int index,const int& element);
	void output();
	//个性化设计
	void clear();
	void push_back(int ans_0); 
	void setsize(int size);
	void remove(int left,int right);
	int& operator[](int index);
	friend bool operator==(list& st,list& at);
	friend bool operator!=(list& st,list& at);
	friend bool operator<(list& st,list& at);
	void leftshift(int it);
//	void reverse();//不适合循环链表 
};

list::list(int size)//ok
{
	listsize = size;
	head = new node(0);
}

list::list(const list& st)//ok
{
	listsize = st.listsize;
	node* first = st.head;
	head = new node(first->ans);
	node* head_0 = head;
	first = first->next;
	for(int i=0;i<listsize;i++)
	{
		head_0->next = new node(first->ans);
		head_0 = head_0->next;
		first = first->next;
	}
	head_0->next = head->next;
}

list::~list()
{
	for (int i = 0; i<listsize; i++)
		erase();
	delete head;
}

void list::erase(int index)//ok
{
	if(index>0)
	{
		if(index<listsize && index==1)
		{
			node* st = head;
			node* at = head;
			for(int i=0;i<listsize;i++)
				at = at->next;
			
		}
	}
		else
			cout << "Wrong!" << endl;
}

int& list::get(int index) const//OK
{
	if (index <= listsize)
	{
		node* st = head->next;
		for (int i = 1; i<index; i++)
			st = st->next;
		return st->ans;
	}
	else
		cout << "wrong!" << endl;
}

int list::indexof(const int& ans)//ok
{
	node* st = head->next;
	int i = 1;
	while (i != listsize)
	{
		if (st->ans == ans)
			break;
		else
			i++;
		st = st->next;
	}
	return i;
}

void list::insert(int index, const int& ans)//ok!
{
	if (index<listsize && index>0)
	{
		node* st = head;
		for (int i = 0; i<index; i++)
			st = st->next;
		st->next = new node(ans, st->next);
		listsize++;
	}
	else if(index<listsize && index==0)
	{
		node* st = head;
		node* at = head;
		for(int i=0;i<listsize;i++)
			at = at->next;
		st->next = new node(ans,st->next);
		at->next = head->next;
		listsize++;
	}
	else if (index == listsize)
	{
		node* st = head;
		for (int i = 0; i<listsize; i++)
			st = st->next;
		st->next = new node(ans,head->next);
		listsize++;
	}
	else
		cout << "Wrong!" << endl;
}

void list::output()//ok
{
	node* st = head->next;
	for(int i=0;i<listsize;i++)
	{
		cout << st->ans << ends;
		st = st->next;
	}
}


void list::push_back(int ans_0)//ok
{
	node* st = head;
	for (int i = 0; i<listsize; i++)
		st = st->next;
	st->next = new node(ans_0,head->next);
	listsize++;
}

void list::setsize(int size)//ok
{
	if(listsize>size)
	{
		int res = listsize-size;
		for(int i=0;i<res;i++)
			erase(listsize);
	}
}

void list::remove(int left,int right)//ok
{
	int res = right-left;
	for(int i=0;i<=res;i++)
		erase(left);	
}

int& list::operator[](int index)//ok
{
	node* st = head;
	for(int i=0;i<index;i++)
		st = st->next;
	return st->ans;
}

bool operator==(list& st,list& at)//ok
{
	if(st.listsize==at.listsize)
	{
		node* s = st.head->next;
		node* a = at.head->next;
		for(int i=0;i<st.listsize;i++)
		{
			if(s->ans == a->ans)
				continue;
			else
				return false;
		}
		return true;
	}
	else
		return false;	
}

bool operator!=(list& st,list& at)//ok
{
	return !(st==at);
}

bool operator<(list& st,list& at)//ok
{
	if(st.listsize==at.listsize)
	{
		node* s = st.head->next;
		node* a = at.head->next;
		for(int i=0;i<st.listsize;i++)
		{
			if(s->ans < a->ans)
				continue;
			else
				return false;
		}
		return true;
	}
	else
		return false;
	
}

void list::leftshift(int it)//ok
{
	int x = listsize-it;
	for(int i=0;i<x;i++)
		head = head->next;
}

int main()
{
	list ch(0);
	for(int i = 1; i<=10; i++)
		ch.push_back(i);
	ch.erase(1);
	ch.output();
	return 0;
}

