#include<iostream>
using namespace std;

struct node
{
	friend class listqueue;
	int ans;
	node* next;
	node(int ans_,node* next_=NULL):ans(ans_),next(next_){};
};

class listqueue
{
private:
	node* head;
	int queuesize;
public:
	listqueue(int size=0);
	~listqueue();
	
	bool empty(){return queuesize==0;};
	int size(){return queuesize;};
	int& front(){return head->next->ans;};
	int& back();
	void pop();
	void push(int key);
	void output();
}; 

listqueue::listqueue(int size)
{
	queuesize = 0;
	head = new node(0);
} 

listqueue::~listqueue()
{
	while(head!=NULL)
	{
		node* st = head->next;
		delete head;
		head = st;
	}
}

void listqueue::pop()
{
	node* st = head->next;
	head->next = head->next->next;
	delete st;
	queuesize--;
}

void listqueue::push(int key)
{
	node* st = head;
	for(int i=0;i<queuesize;i++)
		st = st->next;
	st->next = new node(key);
	queuesize++;
}

int& listqueue::back()
{
	node* st = head;
	for(int i=0;i<queuesize;i++)
		st = st->next;
	return st->ans;
} 

void listqueue::output()
{
	node* sql = head->next;
	while(sql!=NULL)
	{
		cout << sql->ans << ends;
		sql = sql->next;
	}
}

int main()
{
	listqueue st(0);
	for(int i=0;i<10;i++)
		st.push(i);
	st.pop();
	st.output();
	return 0;
}











































































