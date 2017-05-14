#include<iostream>
#include<list>
#include<iterator>
using namespace std;

class binary;

struct node
{
	friend class binary;
	int element;
	node* left;
	node* right;
	node(){left = right = NULL;};
	node(int elemeent)
	

