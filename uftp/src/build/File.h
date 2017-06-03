/*************************************************************************
	> File Name: File.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月27日 星期一 18时04分42秒
 ************************************************************************/

#ifndef _FILE_H
#define _FILE_H

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

//约定只能使用全局地址

namespace unet
{
    namespace file
    {
        class File final
        {
            friend bool operator==(const File& lhs,const File& rhs);
            enum OperatorType{O_WRITE,O_C_WRITE,O_READ,N_WRITE};

            public:
                explicit File(const char* filename_,OperatorType type_);
                explicit File(const std::string& filename_,OperatorType type_);

                File(File& lhs);
                File(File&& lhs);
                File& operator=(File& lhs);
                File& operator=(File&& lhs);
                ~File();  

                const std::string& getFilename() const 
                {return filename;}
                
                const std::string& getGlobalFilename() const
                {return g_filename;}

                const std::string& getFileowner() const
                {return fileowner;};

                const std::string getFiletype() const
                {return filetype;};
                
                void setSeekZero() const
                {};

                int getFd() const
                {return fd;}
                
                bool isOpened() const
                {return opened;};

            private:
                void init();
                int switchOperatorType(OperatorType type_);

            private:
                int fd;
                bool opened;
                std::string filename;
                std::string g_filename;
                std::string fileowner;
                std::string filetype;
                OperatorType type;
        };
        
        bool operator==(const File& lhs,const File& rhs)
        {
            if(lhs.fd == rhs.fd && lhs.g_filename == rhs.g_filename && lhs.filetype == rhs.filetype)
                return true;
            else
                return false;
        }

        int readn(int fd,char* cptr,size_t nbytes);
        int writen(int fd,char* cptr,size_t nbytes);

        void readn(const File& lhs,char* cptr,size_t nbytes);
        void writen(const File& lhs,char* cptr,size_t nbytes);
    }
}
            
#endif



