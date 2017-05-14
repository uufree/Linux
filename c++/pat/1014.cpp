#include<iostream>
#include<string>
#include<vector>
#include<cctype>
#include<map>
using namespace std;

int main()
{
	string word;
	vector<string> vi;
	for(int i=0;i<4;i++)
	{
		cin >> word;
		int size = word.size();
		if(word.empty() || size>60)
			return 0;
		for(int j=0;j<size;j++)
		{
			if(word[j]==' ')
				return 0;
		}
		vi.push_back(word);
	}
	

	char key1,key2;
	int key3;
	string lhs = vi[0];
	string rhs = vi[1];
	int size = lhs.size()<rhs.size()?lhs.size():rhs.size();
	int i;
	for(i=0;i<size;i++)
	{
		if(lhs[i] == rhs[i] && isupper(lhs[i]))
		{
			key1 = lhs[i];
			break;
		}
	}
	
	for(;i<size;i++)
	{
		if(lhs[i] == rhs[i] && isupper(lhs[i]))
			key2 = lhs[i];
	}

	lhs = vi[2];
	rhs = vi[3];
	size = lhs.size()<rhs.size()?lhs.size():rhs.size();
	for(int i=0;i<size;i++)
	{
		if(lhs[i]==rhs[i] && isalpha(lhs[i]))
		{
			key3 = i;
			break;
		}
	}

	map<char,string> one;
	map<char,string> two;

	one.insert(make_pair('A',"MON"));
	one.insert(make_pair('B',"THE"));
	one.insert(make_pair('C',"WED"));
	one.insert(make_pair('D',"THU"));
	one.insert(make_pair('E',"FRI"));
	one.insert(make_pair('F',"SAT"));
	one.insert(make_pair('G',"SUN"));

	two.insert(make_pair('0',"0"));
	two.insert(make_pair('1',"1"));
	two.insert(make_pair('2',"2"));
	two.insert(make_pair('3',"3"));
	two.insert(make_pair('4',"4"));
	two.insert(make_pair('5',"5"));
	two.insert(make_pair('6',"6"));
	two.insert(make_pair('7',"7"));
	two.insert(make_pair('8',"8"));
	two.insert(make_pair('9',"9"));
	two.insert(make_pair('A',"10"));
	two.insert(make_pair('B',"11"));
	two.insert(make_pair('C',"12"));
	two.insert(make_pair('D',"13"));
	two.insert(make_pair('E',"14"));
	two.insert(make_pair('F',"15"));
	two.insert(make_pair('G',"16"));
	two.insert(make_pair('H',"17"));
	two.insert(make_pair('I',"18"));
	two.insert(make_pair('J',"19"));
	two.insert(make_pair('K',"20"));
	two.insert(make_pair('L',"21"));
	two.insert(make_pair('M',"22"));
	two.insert(make_pair('N',"23"));

	cout << one[key1] << " " << two[key2] << ":";
	if(key3<10)
		cout << "0" << key3;
	else
		cout << key3;

	return 0;
}



























		


