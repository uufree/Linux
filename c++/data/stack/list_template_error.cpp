//ÄªÃûÆäÃî£¡£¡£¡£¡ 
#include<iostream>
#include<string>
using namespace std;

template<typename T>
class liststack;

template<typename T>
struct node
{
	friend class liststack<T>;
	T ans;
	node<T>* next;
	node(T ans_,node<T>* next_=NULL):ans(ans_),next(next_){};
};

template<typename T>
class liststack
{
private:
	node<T>* head;
	int listsize;
public:
	liststack(int size=0);
	~liststack();
	bool empty(){return listsize==0;};
	int size(){return listsize;};
	void push(T& key);
	void pop();
	void output();
};

template<typename T>
liststack<T>::liststack(int size)
{
	listsize = size;
	head = new node<T>(0);
}

template<typename T>
liststack<T>::~liststack()
{
	while(head!=NULL)
	{
		node<T>* st = head->next;
		delete head;
		head = st;
	}
}

template<typename T>
void liststack<T>::push(T& key)
{
	node<T>* st = head;
	for(int i=0;i<listsize;i++)
		st = st->next;
	st->next = new node<T>(key);
	listsize++;
}

template<typename T>
void liststack<T>::pop()
{
	node<T>* st = head;
	for(int i=1;i<listsize;i++)
		st = st->next;
	node<T>* at = st->next;
	delete at;
	listsize--;
}

template<typename T>
void liststack<T>::output()
{
	node<T>* sql = head->next;
	while(sql!=NULL)
	{
		cout << sql->ans << ends;
		sql = sql->next;
	}
}


int main()
{
	liststack<int> as();
	as.push(9);
	as.output();
	cout << as.size() << endl;
	return 0;
}




