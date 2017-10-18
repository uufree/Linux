/*************************************************************************
	> File Name: Directory.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月19日 星期五 16时33分18秒
 ************************************************************************/

#include"Directory.h"
#include"error.h"
#include<dirent.h>

namespace unet
{
    namespace file
    {
        Directory::Directory(const char* path) : directorypath(path)
        {
            update(directorypath);
        }
        
        Directory::Directory(const std::string& lhs) : directorypath(lhs)
        {
            update(directorypath);
        };

        Directory::Directory(const Directory& lhs) : directorypath(lhs.directorypath)
        {
            update(directorypath);
        }

        Directory::Directory(Directory&& lhs) : directorypath(std::move(lhs.directorypath))
        {
            update(directorypath);
        }

        Directory& Directory::operator=(const Directory& lhs)
        {
            if(lhs == *this)
                return *this;
            
            directorypath = lhs.directorypath;
            update(directorypath);
            return *this;
        }

        Directory& Directory::operator=(Directory&& lhs)
        {
            if(*this == lhs)
                return *this;
                
            directorypath = std::move(lhs.directorypath);
            directorylist.clear();

            update(directorypath);
            return *this;
        }

        Directory::~Directory()
        {}
    
        void Directory::update(const std::string& lhs)
        {
            struct dirent* drip = NULL;
            DIR* dp = ::opendir(lhs.c_str());
            if(dp == NULL)
                unet::handleError(errno);         

            while((drip=readdir(dp)) != NULL)
            {
                if((strcmp(drip->d_name,".")==0) || (strcmp(drip->d_name,"..")==0))
                    continue;
            
                directorylist.push_back(drip->d_name);
                
                directoryBuffer.append(drip->d_name);
                directoryBuffer.append("\t");
            }
        }

        void Directory::addInDirectoryList(const std::string& filename)
        {
            directorylist.push_back(filename);
            directoryBuffer.append(filename);
            directoryBuffer.append("\t");
        }
        
        bool operator==(const Directory& lhs,const Directory& rhs)
        {
            if(lhs.directorypath == rhs.directorypath)
                return true;
            else
                return false;
        }
    }
}


    
