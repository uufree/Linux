#include<iostream>
using namespace std;

struct node
{
	int ans;
	node* next;
	node(int ans_,node* next_=NULL):ans(ans_),next(next_){};
};

node* head = new node(0);
int listsize = 0;

//注意清点桶的个数 

void binsert();

int main()
{
	int key[10] = {6217,3289,4312,6434,7590,6821,7493,8695,7897,7456};
	node* copy = head;
	for(int i=0;i<10;i++)
	{
		copy->next = new node(key[i]);
		copy = copy->next;
		listsize++;
	}
	

	binsert();
	
	
	node* sql = head->next;
	while(sql!=NULL)
	{
		cout << sql->ans << ends;
		sql = sql->next;
	}
	return 0;
} 


void binsert()
{
	{	
	node** bottom = new node*[10];//100根据最大数值可修改 
	node** top = new node*[10];
	for(int i=0;i<10;i++)
		bottom[i] = NULL;
		

//数据分桶	
	for(;head!=NULL;head=head->next)
	{
		int x = (((head->ans)%1000)%100)%10;
		if(bottom[x]==NULL)
			bottom[x] = top[x] = head;
		else
		{
			top[x]->next = head;
			top[x] = head;
		}
	}
	
//连接桶
	node* at = NULL;
	for(int i=0;i<10;i++)
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
	delete [] bottom;
	delete [] top;
}

{
	node** bottom = new node*[10];//100根据最大数值可修改 
	node** top = new node*[10];
	for(int i=0;i<10;i++)
		bottom[i] = NULL;
		

//数据分桶	
	for(;head!=NULL;head=head->next)
	{
		int x = (((head->ans)%1000)%100)/10;
		if(bottom[x]==NULL)
			bottom[x] = top[x] = head;
		else
		{
			top[x]->next = head;
			top[x] = head;
		}
	}
	
//连接桶
	node* at = NULL;
	for(int i=0;i<10;i++)
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
	delete [] bottom;
	delete [] top;
}

{
	node** bottom = new node*[10];//100根据最大数值可修改 
	node** top = new node*[10];
	for(int i=0;i<10;i++)
		bottom[i] = NULL;
		

//数据分桶	
	for(;head!=NULL;head=head->next)
	{
		int x = ((head->ans)%1000)/100;
		if(bottom[x]==NULL)
			bottom[x] = top[x] = head;
		else
		{
			top[x]->next = head;
			top[x] = head;
		}
	}
	
//连接桶
	node* at = NULL;
	for(int i=0;i<10;i++)
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
	delete [] bottom;
	delete [] top;
}

{
	node** bottom = new node*[10];//100根据最大数值可修改 
	node** top = new node*[10];
	for(int i=0;i<10;i++)
		bottom[i] = NULL;
		

//数据分桶	
	for(;head!=NULL;head=head->next)
	{
		int x = (head->ans)/1000;
		if(bottom[x]==NULL)
			bottom[x] = top[x] = head;
		else
		{
			top[x]->next = head;
			top[x] = head;
		}
	}
	
//连接桶
	node* at = NULL;
	for(int i=0;i<10;i++)
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
	delete [] bottom;
	delete [] top;
}

}










 
