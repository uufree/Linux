#include<vector>
#include<iterator>
#include<iostream>
using namespace std;
int main()
{
	vector<int> vi;
	for(int i=0;i<10;i++)
		vi.push_back(i);
	vector<int>::iterator iter = vi.begin();
	vi.erase(iter);
	for(vector<int>::reverse_iterator r_iter = vi.rbegin();r_iter!=vi.rend();r_iter++)
		cout << *r_iter << endl;
	return 0;
}


