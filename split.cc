/*************************************************************************
	> File Name: split.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月03日 星期六 14时31分33秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<vector>
#include<time.h>

void split(const std::string& lhs,const std::string& rhs,std::vector<std::string>& vecStr)
{
    size_t last = 0;
    size_t index = lhs.find_first_of(rhs,last);
    while(index != std::string::npos)
    {
        vecStr.push_back(lhs.substr(last,index-last));
        last = index + 1;
        index = lhs.find_first_of(rhs,last);
    }

    if((index-last) > 0)
        vecStr.push_back(lhs.substr(last,index-last));
}

int main(int argc,char** argv)
{
    std::vector<std::string> vec;
    time_t Time = time(NULL);
    std::string strTime = ctime(&Time);
    std::cout << strTime << std::endl;

    split(strTime," ",vec);
    
    for(size_t i=0;i<vec.size();++i)
        std::cout << i << " : " << vec[i] << std::endl;

    return 0;
}
