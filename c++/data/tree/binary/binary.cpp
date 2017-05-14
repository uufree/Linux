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
	node(int element_,node* left_=NULL,node* right_=NULL):element(element_),left(left_),right(right_){};
};

class binary
{
	private:
		//私有方法是为了继承的时候方便使用
		node* boot;
		int treesize;
		void preorderhelp(node* st) const;
		void inorderhelp(node* st) const;
		void postorderhelp(node* st) const;
		void erasehelp(node* st);
		int heighthelp(node* st) const;
		void copyhelp(node* st,node* at) const;

	public:
		binary(int data) {boot = new node(data);treesize=1;};
		binary() {boot = NULL;treesize=1;};
		~binary() {erasehelp(boot);};
		binary(const binary& bin);

		bool empty() const  {return treesize==0;};
		int size() const  {return treesize;};
		node* gettop() const {return boot;};
		void preorder() const;
		void inorder() const;
		void postorder() const;
//		void levelorder();
		void erase();
		int height() const;
		void insertleft(node* lhs,int data);
		void insertright(node* rhs,int data);
};


void binary::copyhelp(node* st,node* at) const
{
	if(st!=NULL)
	{
		at = new node(st->element);
		copyhelp(st->left,at->left);
		copyhelp(st->right,at->right);
	}
}


binary::binary(const binary& bin)
{
	treesize = bin.treesize;
	boot = bin.boot;
	copyhelp(bin.boot,boot);
}


void binary::preorderhelp(node* st) const
{
	if(st!=NULL)
	{
		cout << st->element << endl;
		preorderhelp(st->left);
		preorderhelp(st->right);
	}
}

void binary::preorder() const
{
	preorderhelp(boot);
}

void binary::inorderhelp(node* st) const
{
	if(st!=NULL)
	{
		inorderhelp(st->left);
		cout << st->element << endl;
		inorderhelp(st->right);
	}
}

void binary::inorder() const
{
	inorderhelp(boot);
}

void binary::postorderhelp(node* st) const
{
	if(st!=NULL)
	{
		postorderhelp(st->left);
		postorderhelp(st->right);
		cout << st->element << endl;
	}
}

void binary::postorder() const
{
	postorderhelp(boot);
}

/*void binary::levelorder() 
{
	list<node*> list;
	node* st = boot;

	while(st!=NULL)
	{
		cout << st->element << endl;

		if(st->left!=NULL)
			list.push_back(st->left);
		if(st->right!=NULL)
			list.push_back(st->right);

		list<node* >::iterator iter = list.begin();
        :cn
		list.erase(iter);
	}
}*/




void binary::erasehelp(node* st)
{
	if(st!=NULL)
	{
		erasehelp(st->left);
		erasehelp(st->right);
		delete st;
		st = NULL;
	}
}

void binary::erase()
{
	erasehelp(boot);
}

int binary::heighthelp(node* st) const
{
	if(st==NULL)
		return 0;
	else
	{
		int lheight,rheight;
		lheight = heighthelp(st->left);
		rheight = heighthelp(st->right);
		return (lheight>rheight)?lheight+1:rheight+1;
	}
}

int binary::height() const
{
	return heighthelp(boot);
}
	
void binary::insertleft(node* lhs,int data)
{
	if(lhs->left!=NULL)
		return;
	else
	{
		node* child = new node(data);
		if(lhs->left!=NULL)
			child->left = lhs->left;
		lhs->left = child;
		treesize++;
		return;
	}
}

void binary::insertright(node* rhs,int data)
{
	if(rhs->right!=NULL)
		return;
	else
	{
		node* child = new node(data);
		if(rhs->right!=NULL)
			child->right = rhs->right;
		rhs->right = child;
		treesize++;
		return;
	}
}

int main()
{
	int res = 1000;
	binary tree(res);
	
	for(int i=0;i<10;i++)
	{
		if(i%2)
		{
			node* st = tree.gettop();
			tree.insertleft(st,i);
			tree.insertright(st,i+1);
			st = st->left;
		}
		else
		{
			node* st = tree.gettop();
			tree.insertright(st,i);
			tree.insertleft(st,i+1);
			st = st->right;
		}
	}

	cout << "Preorder:" << endl;
	tree.preorder();
	
//	binary bin(tree);
//	bin.preorder();

//	cout << "bin's size: " << bin.size() << endl;
//	cout << "bin's height: " << bin.height() << endl; 
//	cout << "inorder:" << endl;
//	tree.inorder();

//	cout << "postorder:" << endl;
//	tree.postorder();

	cout << "levelorder:" << endl;
	tree.levelorder();

	cout << "tree's height:" << tree.height() << endl;

	cout << "tree's size:" << tree.size() << endl;

//	tree.erase();
//	tree.preorder();
//	cout << endl;

	return 0;

}
































