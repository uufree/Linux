#include<iostream>
using namespace std;
void tower(int n,int x,int y,int z); 
int main()
{
	tower(3,1,2,3);
	return 0;
} 

void tower(int n,int x,int y,int z)
{
	if(n>0)
	{
		tower(n-1,x,z,y);
		cout << "Move " << x << " To " << y << endl;
		tower(n-1,z,y,x);
	}
}
