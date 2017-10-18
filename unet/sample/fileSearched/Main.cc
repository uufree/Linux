/*************************************************************************
	> File Name: Main.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年09月19日 星期二 18时39分14秒
 ************************************************************************/

#include<iostream>
#include"FileSearched.h"

int main(int argc,char** argv)
{
    FileSearched file("/home/uuchen/unet");

    const std::vector<std::string>& strList = file.getFileNameList();
    
    for(auto iter=strList.begin();iter != strList.end();++iter)
        std::cout << *iter << std::endl;
     
    return 0;
}
