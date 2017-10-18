/*************************************************************************
	> File Name: FileSearched.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年09月11日 星期一 16时44分13秒
 ************************************************************************/

#ifndef _FILESEARCHED_H
#define _FILESEARCHED_H

#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string>
#include<vector>

class FileSearched final
{
    friend bool operator==(const FileSearched& lhs,const FileSearched& rhs);

    public:
        explicit FileSearched(const std::string& directoryName);
        FileSearched(const FileSearched& lhs);
        FileSearched(FileSearched&& lhs);
        FileSearched& operator=(const FileSearched& lhs);
        FileSearched& operator=(FileSearched&& lhs);
        ~FileSearched();

        const std::vector<std::string>& getFileNameList();

        const std::string& getDirectoryPath()
        {return directoryPath;};

    private:
        int update();
        
    private:
        std::string directoryPath;
        std::vector<std::string> fileNameList;
};

#endif
