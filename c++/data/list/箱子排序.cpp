#include<iostream>
using namespace std;

class list;

struct node
{
	friend class list;
	int ans;
	node* next;
	node(int ans_, node* next_ = NULL) :ans(ans_), next(next_) {};
};

class list
{
private:
	node* head;
	int listsize;
public:
	//三五法则 
	list(int size = 0);
	list(const list & st);
	~list();
	//ADT
	bool empty() { return listsize == 0; };
	int size() { return listsize; }
	int& get(int index) const;
	int indexof(const int& element);
	void erase(int index = 1);
	void insert(int index, const int& ans);
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
	void reverse(); 
	void binsert();
};

bool operator==(list& st,list& at);
bool operator!=(list& st,list& at);

//基于类外的箱子排序 
void binsert(list& st,int range)
{
	list* bin = new list[range+1];
	
	int number = st.size();
	for(int i=0;i<=range;i++)
	{
		int x = st.get(1);
		st.erase(1);
		bin[x].insert(0,x);
	}
	
	
	for(int i=range;i>=0;i--)
	{
		while(bin[i].size()!=0)
		{
			int x = bin[i].get(1);
			bin[i].erase(1);
			st.insert(0,x);
		}
	}
		
	delete [] bin; 
} 




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
	while (first != NULL)
	{
		head_0->next = new node(first->ans);
		head_0 = head_0->next;
		first = first->next;
	}
	head_0->next = NULL;
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
		node* st = head;
		for (int i = 1; i<index; i++)
			st = st->next;
		if (index<listsize)
		{
			node* at = st->next;
			st->next = st->next->next;
			delete at;
			listsize--;
		}
		else if (index == listsize)
		{
			node* at = st->next;
			st->next = NULL;
			delete at;
			listsize--;
		}
		else
			cout << "wrong!" << endl;
	}
	else
		cout << "wrong!" << endl;
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
	if (index<listsize)
	{
		node* st = head;
		for (int i = 0; i<index; i++)
			st = st->next;
		st->next = new node(ans, st->next);
		listsize++;
	}
	else if (index == listsize)
	{
		node* st = head;
		for (int i = 0; i<listsize; i++)
			st = st->next;
		st->next = new node(ans);
		listsize++;
	}
	else
		cout << "wrong!" << endl;
}

void list::output()//ok
{
	node* st = head->next;
	while (st != NULL)
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
	st->next = new node(ans_0);
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
	if(it<listsize)
	{
		for(int i=0;i<it;i++)
		{
			node* at = head;
			for(int i=1;i<listsize;i++)
				at = at->next;
			node* qt = at->next;
			at->next = NULL;
			insert(0,qt->ans);
			listsize--;
		}
	}
}

void list::reverse()
{
	for(int j=0;j<listsize-1;j++)
	{
		node* st = head;
		for(int i=1;i<listsize;i++)
			st = st->next;	
		node* at = st->next;
		st->next = NULL;
		insert(j,at->ans);
		listsize--;
	}
}

//基于类内的箱子排序,待理解连接部分，关键在于创建多少桶 
void list::binsert()
{
	int listsize = 100;
	node** bottom = new node* [listsize];//十个保存首尾指针的数组 
	node** top = new node* [listsize];
	for(int i=0;i<listsize;i++)
		bottom[i] = NULL;
			
	for(;head!=NULL;head=head->next)
	{
		int x = head->ans;
		if(bottom[x] == NULL)
			bottom[x] = top[x] = head;
		else
		{
			top[x]->next = head;
			top[x] = head;
		}		
	}
	
	node* at = NULL;
	for(int i=0;i<listsize;i++)
	{
		if(bottom[i]!=NULL)
		{
			if(at==NULL)
				head = bottom[i];
			else
				at->next = bottom[i];
			at = top[i];	
		}
		if(at!=NULL)
			at->next = NULL;
	} 
}

int main()
{
	int key[10] = {10,67,98,56,94,76,24,10,73,65};
//	int key[10] = {5,4,7,2,9,8,3,10,9,5};
	list ch;
	for(int i=0;i<10;i++)
		ch.push_back(key[i]);
	ch.binsert();
	ch.output();
	return 0;
}























