#include<iostream>
#include<string>
using namespace std;

struct node
{
	friend class map;
	string first;//name
	string second;//number
	node* next;
	node(string first_="no name",string second_="no number",node* next_=NULL):first(first_),second(second_),next(next_){};
};

class map
{
	private:
		node* head;
		int mapsize;
	public:
		map();
		~map();

		bool empty(){return mapsize==0;};
		int size(){return mapsize;};
		string find (string search);
		void erase(node& search);
		void push(node& search);
		void output();
};

map::map()
{
	mapsize = 0;
	head = new node();
}

map::~map()
{
	while(head!=NULL)
	{
		node* st = head->next;
		delete head;
		head = st;
	}
}

string map::find(string search)
{
	node* st = head;
	while(st!=NULL)
	{
		if(st->first == search)
			break;
		else
			st = st->next;
	}
	return st->second;
}

void map::erase(node& search)
{
	node* st = head;
	while(st->next != NULL)
	{
		if(st->first == search.first && st->next!=NULL)
		{
			node* at = st->next;
			st->next = at->next;
			delete at;
		}
		else
			delete st;
	}
	mapsize--;
}

void map::push(node& search)
{
node* st = head;
for(int i=0;i<mapsize;i++)
	st = st->next;
st->next = new node(search.first,search.second);
	mapsize++;
}

void map::output()
{
	node* st = head;
	for(int i=0;i<mapsize;i++)
		cout << st->first << "--" << st->second << endl;
}

int main()
{
	map dirc;
	string name = "will";
	string number = "14154805";
	node st(name,number);
	dirc.push(st);
	node at("mark","12345678");
	dirc.push(at);
//	cout << find("will") << endl;
	cout << dirc.size() << endl;
	dirc.output();
	cout << endl << endl;
	dirc.erase(at);
	dirc.output();
	return 0;
}




































