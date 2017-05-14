#include<iostream>
#include<string>
using namespace std;

struct position
{
	friend class liststack;
	friend struct node;
	int row;
	int col;
	position(int row_ = 0, int col_ = 0) :row(row_), col(col_) {};
};

struct node
{
	friend class liststack;
	position path;
	node* next;
	node(position path_, node* next_ = NULL) :path(path_), next(next_) {};
};

class liststack
{
private:
	node* head;
	int listsize;
public:
	//ÈýÎå·¨Ôò 
	liststack(int size = 0);
	~liststack();
	//ADT
	bool empty() { return listsize == 0; };
	int size() { return listsize; }
	void push(position& at);
	void pop();
	//	void output();
	position top();
};

liststack::liststack(int size)//ok
{
	position st(0, 0);
	listsize = size;
	head = new node(st);
}

liststack::~liststack()
{
	while (head != NULL)
	{
		node* st = head->next;
		delete head;
		head = st;
	}
}

void liststack::push(position& at)
{
	node* st = head;
	for (int i = 0; i<listsize; i++)
		st = st->next;
	st->next = new node(at);
	listsize++;
}

void liststack::pop()
{
	node* st = head;
	for (int i = 1; i<listsize; i++)
		st = st->next;
	node* at = st->next;
	delete at;
	listsize--;
}

/*void liststack::output()
{
node* sql = head->next;
while(sql!=NULL)
{
cout << sql->path->row << "." << sql->path->col << endl;
sql = sql->next;
}
}*/

position liststack::top()
{
	node* at = head;
	for (int i = 0; i<listsize; i++)
		at = at->next;
	return at->path;
}

bool maze[5][5] = {1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,1,1,1};//ÓÐ³ö¿ÚÃÔ¹¬£¬´°¿Ú²»µ¯³ö
//bool maze[5][5] = { 1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,0,1,1,1,1,1,1 };//ÎÞ³ö¿ÚÃÔ¹¬£¬´°¿Úµ¯³ö
liststack path(0);//Õ»µÄÈ«¾ÖÉùÃ÷ 
bool findPath();

int main()
{
	bool res = findPath();
	cout << res << endl;
	return 0;
}

bool findPath()
{
	int size = 3;
	position offset[4];
	offset[0].row = 0; offset[0].col = 1;//ÓÒ 
	offset[1].row = 1; offset[1].col = 0;//ÏÂ 
	offset[2].row = 0; offset[2].col = -1;//×ó 
	offset[3].row = -1; offset[3].col = 0;//ÉÏ


	position here(1, 1);
	maze[1][1] = 1;//·ÀÖ¹»Øµ½Èë¿Ú 
	int option = 0;
	int lastoption = 3;

	while (here.row != size || here.col != size)
	{
		int r, c;
		while (option <= lastoption)
		{
			r = here.row + offset[option].row;
			c = here.col + offset[option].col;
			if (maze[r][c] == 0)
				break;//ÕÒµ½Ò»ÌõÂ·¾¶
			option++;
		}

		if (option <= lastoption)
		{
			path.push(here);
			here.row = r;
			here.col = c;
			maze[r][c] = 1;//¿¿¿¿¿
			option = 0;
		}
		else
		{
			if (path.empty())
				return false;
			position next = path.top();
			path.pop();
			option++;//ÓëÏÂÃæµÄ²»Í¬..
/*			if (next.row == here.row)
				option = 2 + next.row - here.row;
			else
				option = 3 + next.col - here.col;   */
			here = next;
		}
	}
	return true;
}
