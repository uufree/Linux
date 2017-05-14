#include<iostream>
#include<stack>
#include<vector>
#include<iterator>
using namespace std;

struct position
{
	friend struct node;
	friend class listqueue;
	int row;
	int col;
	int sign;
	position(int row_=0,int col_=0,int sign_=0):row(row_),col(col_),sign(sign_){};
	void add(){++sign;};
};

struct node
{
	friend class listqueue;
	position path;
	node* next;
	node(position path_,node* next_=NULL):path(path_),next(next_){};
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
		position& top(){return head->next->path;};
		position& back();
		void pop();
		void push(position& at);
};

listqueue::listqueue(int size)
{
	position st(0,0);
	queuesize = 0;
	head = new node(st);	
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

void listqueue::push(position& at)
{
	node* st = head;
	for(int i=0;i<queuesize;i++)
		st = st->next;
	st->next = new node(at);
	queuesize++;
}

position& listqueue::back()
{
	node* st = head;
	for(int i=0;i<queuesize;i++)
		st = st->next;
	return st->path;
}

bool maze[9][9]={0,0,0,0,0,0,0,0,0,
				 0,1,1,0,1,1,1,1,0,
				 0,1,1,0,0,1,1,1,0,
				 0,1,1,1,1,0,1,1,0,
				 0,1,1,1,0,0,1,1,0,
				 0,0,1,1,1,0,1,1,0,
				 0,0,0,0,1,1,1,1,0,
				 0,0,0,0,1,1,1,1,0,
				 0,0,0,0,0,0,0,0,0};//迷宫，1表示可通过，0表示不可通过

listqueue path(0);
void findpath();
stack<position> ck;//保存最短境结果
vector<position> vi;
position search_vec(int r,int c);

int main()
{
	findpath();
	while(!ck.empty())
	{
		position st = ck.top();
		cout << st.col << "," << st.row << endl;
		ck.pop();
	}
	return 0;
}

void findpath()
{
	position offset[4];
	offset[0].row=0;offset[0].col=1;//右
	offset[1].row=1;offset[1].col=0;//下
	offset[2].row=0;offset[2].col=-1;//左
	offset[3].row=-1;offset[3].col=0;//上
	
	position here(3,2);
	position finish(4,6);
	ck.push(finish);
	here.sign = 1;

	path.push(here);
	while(finish.sign==0)
	{
		int judge=0;
		position st = path.top();
		while(judge<4)
		{
			int r = st.row + offset[judge].row;
			int c = st.col + offset[judge].col;
			if(maze[r][c]==1)
			{
				position next(r,c,++st.sign);
				vi.push_back(next);//vi中保存的是所有是1的位置和节点信息
				path.push(next);
			}
			++judge;
		}
		path.pop();
	}

	int x = finish.sign;
	position search = ck.top();
	int judge = 0;
	while(x!=1)
	{
		int r,c;
		while(judge<4)
		{
			search = search_vec(r,c);
			r = search.row - offset[judge].row;
			c = search.col - offset[judge].col;
			if(search.sign!=0)
				break;
			judge++;
		}

		if(search.sign == x-1)
		{
			ck.push(search);
			search.sign = 0;//关键
			--x;
			r = search.row;
			c = search.col;
			judge = 0;
		}
		else
		{
			if(path.empty())
				return; 
			search = ck.top();
			++x;
			++judge;
			ck.pop();
		}
	}
}
		

position search_vec(int r,int c)
{
	int size = vi.size();
	position result;
	for(int i=0;i<size;i++)
	{
		if(vi[i].row!=r || vi[i].col!=c)
			continue;
		else
		{
			result = vi[i];
			break;
		}
	}
	return result;
}






























