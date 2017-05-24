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

liststack stack[3];
liststack result;
bool scan();
int mix(int a,int b,int c);
int main()
{
	int key[9] = {3,6,9,2,4,7,1,8,5};
	int size = 0;
	while(size!=9)
	{
		int x = key[size];
		if(x==1)
		{
			result.push(x);
			while(scan());
			size++;
			continue;
		}
		
		while(scan());
			
		{
			int a,b,c,a_c,b_c,c_c;
			//得到X与每个的差值 
			{
				if(stack[0].empty())
					a = 0;
				else
					a = stack[0].top();
				if(stack[1].empty())
					b = 0;
				else
					b = stack[1].top();
				if(stack[2].empty())
					c = 0;
				else
					c = stack[2].top();
				if(x<a)
					a_c = a-x;
				else
					a_c = 0;
				if(x<b)
					b_c = b-x;
				else
					b_c = 0;
				if(x<c)
					c_c = c-x;
				else
					c_c=0;
			}
			if(a==0)//等于0优先插入
			{
				stack[0].push(x);
				size++;
				continue;
			}
			if(b==0)
			{
				stack[1].push(x);
				size++;
				continue;
			}
			if(c==0)
			{
				stack[2].push(x);
				size++;
				continue;
			}
			int d = min(a_c,b_c,c_c);
			if(d==a_c && d>0)
			{
				stack[0].push(x);
				size++;
				continue;
			}
			if(d==b_c && d>0)
			{
				stack[1].push(x);
				size++;
				continue;
			}
			if(d==c_c && d>0)
			{
				stack[2].push(x);
				size++;
				continue;
			}		
		}
	}
}

int mix(int a,int b,int c)
{
	if(a>b)
		a=b;
	if(a>c)
		a=c;
	return a;	
}

bool scan()
{
		int a,b,c;
		if(stack[0].empty())
			a = 0;
		else
			a = stack[0].top();
		if(stack[1].empty())
			b = 0;
		else
			b = stack[1].top();
		if(stack[2].empty())
			c = 0;
		else
			c = stack[2].top();
		int x = result.top();	
		if(a == x+1)
		{
			int res = stack[0].top();
			result.push(res);
			stack[0].pop();
			return true;
		}
		if(b==x+1)
		{
			int res = stack[1].top();
			result.push(res);
			stack[1].pop();
			return true;
		}	
		if(c==x+1)
		{
			int res = stack[2].top();
			result.push(res);
			stack[2].pop();
			return true;
		}
		return false;
}
