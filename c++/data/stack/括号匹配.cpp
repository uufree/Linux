//莫名奇妙，算了，不浪费时间了 
#include<iostream>
#include<string>
using namespace std;


struct node
{
	friend class liststack;
	char ch;
	int address;
	node* next;
	node(char ch_,int address_, node* next_ = NULL) :ch(ch_),address(address_), next(next_) {};
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
	void push(char ch_,int address_);
	void pop();
	void output();
	char getch();
	int getin();
};

liststack::liststack(int size)//ok
{
	listsize = size;
	head = new node(0,0);
}

liststack::liststack(const liststack& st)//ok
{
	listsize = st.listsize;
	node* first = st.head;
	head = new node(first->ch,first->address);
	node* head_0 = head;
	first = first->next;
	while (first != NULL)
	{
		head_0->next = new node(first->ch,first->address);
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

void liststack::push(char ch_,int address_)
{
	node* st = head;
	for (int i = 0; i<listsize; i++)
		st = st->next;
	st->next = new node(ch_,address_);
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
		cout << sql->ch << ends << sql->address << endl;
		sql = sql->next;
	}
}

char liststack::getch()
{
	node* at = head;
	for(int i=0;i<listsize;i++)
		at = at->next;
	return at->ch;	
}

int liststack::getin()
{
	node* at = head;
	for(int i=0;i<listsize;i++)
		at = at->next;
	return at->address;	
}

int main()
{
	liststack stack(0);
	string str = "((()))((()))";
	for(int i=0;i<str.size();i++)
	{
		if(str[i] == '(')
			stack.push('(',i);
		if(str[i]==')')
			stack.push(')',i);
	}
	stack.output();
	return 0;
}


