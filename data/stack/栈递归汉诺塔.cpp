//思想不变，仅仅体现每个塔实时的盘子！ 
#include<iostream>
#include<string>
using namespace std;


struct node
{
	friend class liststack;
	int ans;
	node* next;
	node(int ans_, node* next_ = NULL) :ans(ans_), next(next_) {};
};

class liststack
{
private:
	node* head;
	int listsize;
public:
	//三五法则 
	liststack(int size = 0);
	liststack(const liststack & st);
	~liststack();
	//ADT
	bool empty() { return listsize == 0; };
	int size() { return listsize; }
	void push(int key);
	void pop();
	void output();
	int top();
};

liststack::liststack(int size)//ok
{
	listsize = size;
	head = new node(0);
}

liststack::liststack(const liststack& st)//ok
{
	listsize = st.listsize;
	node* first = st.head;
	head = new node(first->ans);
	node* head_0 = head;
	first = first->next;
	while (first != NULL)
	{
		head_0->next = new node(first->ans);
		head_0 = head_0->next;
		first = first->next;
	}
	head_0->next = NULL;
}

liststack::~liststack()
{
	while(head!=NULL)
	{
		node* st = head->next;
		delete head;
		head = st;
	}
}

void liststack::push(int key)
{
	node* st = head;
	for (int i = 0; i<listsize; i++)
		st = st->next;
	st->next = new node(key);
	listsize++;
}

void liststack::pop()
{
	node* st = head;
	for(int i=1;i<listsize;i++)
		st = st->next;
	node* at = st->next;	
	delete at;
	listsize--;
}

void liststack::output()
{
	node* sql = head->next;
	while(sql!=NULL)
	{
		cout << sql->ans << ends;
		sql = sql->next;
	}
}

int liststack::top()
{
	node* at = head;
	for(int i=0;i<listsize;i++)
		at = at->next;
	return at->ans;
}


int main()
{
	liststack st[3];
	for(int i=3;i>0;i--)
		st[1].push(i);
	tower(3,1,2,3);	
	return 0;
}

