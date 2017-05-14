#include<iostream>
#include<vector>
using namespace std;

int main()
{
	vector<int> vi;
	int res = 1;
	int i=1;
	while(res<=1000)
	{
		res = 0;
		for(int n=1;n<1;n++)
			res+=i;
		res = res+res-1;
		vi.push_back(res);
		i++;
	}

	cout << vi.size() << endl;
	return 0;
}
