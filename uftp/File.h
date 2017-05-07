/*************************************************************************
	> File Name: File.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月27日 星期一 18时04分42秒
 ************************************************************************/

#ifndef _FILE_H
#define _FILE_H

//这个类的作用是封装文件描述符的操作，拒绝使用C++提供的stringstream

#include<assert.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string>
#include<string.h>
#include<dirent.h>
#include<iostream>
#include<vector>
#include"Mutex.h"
#include<algorithm>

namespace unet
{
    class File final
    {
        public:
            explicit File(const char* filename_);
            File(const File&) = delete;
            File& operator=(const File&) = delete;
            ~File()
            {
                assert(!closed);
                assert(::close(fd) == 0);
            }

            void readn(char* cptr,size_t nbytes);
            
            int getReadSize() const
            {return readsize;};
            

            void writen(char* cptr,size_t nbytes);
    
            int getWriteSize() const
            {return writesize;};

            const std::string& getFilename() const 
            {return filename;}

            int getFd() const
            {return fd;}

        private:
            int fd;
            std::string filename;
            bool opened,closed;
            int readsize,writesize;
    };

    class Directory final
    {
        public:
            explicit Directory(const char* path);
            Directory(const Directory& lhs) = delete;
            Directory& operator=(const Directory& lhs) = delete;
            ~Directory();
            
            char* getDirBuffer() const;
            void update();
            void addInDirectoryList(const char* filename);
            bool inDirectoryList(const char* filename);

        private:
            std::string directorypath;
            std::vector<std::string> directorylist;
            char* directorybuffer;
            thread::MutexLock lock;
    };
}
            
#endif



