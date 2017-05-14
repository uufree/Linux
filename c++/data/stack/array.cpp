#include<iostream>
using namespace std;

class arraystack
{
private:
	int* head;
	int* back;//back为指向实际数字后的一个指针 
	int stacksize;
	int consize;
public:
	arraystack(int consize_ = 10);
	~arraystack() { delete[] head; };

	bool empty() const { return stacksize == 0; };
	int size() const { return stacksize; };
	void output();
	int& frontnum() const { return *head;};
	int& backnum() { return *(--back);};
	void push(int key);//重新分配内存时出错 
	void pop();
};

arraystack::arraystack(int consize_)
{
	consize = consize_;
	head = new int[consize];
	stacksize = 0;
	back = head;
}

void arraystack::output()
{
	for (int i = 0; i<stacksize; i++)
		cout << *(head + i) << ends;
}

void arraystack::push(int key)
{
	if (stacksize < consize)
	{
		*back = key;
		back++;
	}
	else
	{
		consize = consize+10;
		int* first = new int[consize];
		for (int i = 0; i < consize; i++)
			*(first + i) = *(head + i);
		delete [] head;	
		head = first;
		back = first + stacksize;
		*back = key;
		back++;
	}
	stacksize++;
}

void arraystack::pop()
{
	--back;
	--stacksize;
}

int main()
{
	arraystack stack(10);
	for (int i = 0; i<20; i++)
		stack.push(i + 1);
	stack.pop();	
	cout << stack.size() << endl;	
	stack.output();
	return 0;
}
