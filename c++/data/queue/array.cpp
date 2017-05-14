#include<iostream>
using namespace std;

class arrayqueue
{
private:
	int* head;
	int* back;//back为指向实际数字后的一个指针 
	int queuesize;
	int consize;
public:
	arrayqueue(int consize_ = 10);
	~arrayqueue() { delete[] head; };

	bool empty() const { return queuesize == 0; };
	int size() const { return queuesize; };
	void output();
	int& frontnum() { return *head; };
	int& backnum() { return *(--back); };
	void push(int key);//重新分配内存时出错 
	void pop();
};

arrayqueue::arrayqueue(int consize_)
{
	consize = consize_;
	head = new int[consize];
	queuesize = 0;
	back = head;
}

void arrayqueue::output()
{
	for (int i = 0; i<queuesize; i++)
		cout << *(head + i) << ends;
}

void arrayqueue::push(int key)
{
	if (queuesize < consize)
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
		back = first + queuesize;
		*back = key;
		back++;
	}
	queuesize++;
}

void arrayqueue::pop()
{
	int* first = new int[consize];
	copy(head+1,back,first);
	delete [] head;
	head = first;
	back = head+consize-1;
	queuesize--;
}

int main()
{
	arrayqueue queue(10);
	for (int i = 0; i<20; i++)
		queue.push(i + 1);
	queue.pop();	
	cout << queue.size() << endl;	
	queue.output();
	return 0;
}



















