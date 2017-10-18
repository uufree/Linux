/*************************************************************************
	> File Name: Buffer.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月14日 星期二 19时28分46秒
 ************************************************************************/

#include"Buffer.h"
#include"error.h"

#include<iostream>
#include<string.h>
#include<unistd.h>
#include<sys/sendfile.h>
#include<sys/stat.h>

namespace unet
{
    namespace net
    {
        Buffer::Buffer(int fd_) : 
            confd(fd_),
            buffer()
        {};

        Buffer::Buffer(Buffer&& lhs) : 
            confd(lhs.confd),
            buffer(std::move(lhs.buffer))
        {};

        Buffer& Buffer::operator=(Buffer&& lhs)
        {
            confd = lhs.confd;
            buffer = std::move(lhs.buffer);

            return *this;
        }
        
        Buffer::~Buffer()
        {};

        void Buffer::swap(Buffer& lhs)
        {
            std::swap(confd,lhs.confd);
            std::swap(buffer,lhs.buffer);
        }
        
        bool operator==(const Buffer& lhs,const Buffer& rhs)
        {
            if(lhs.confd == rhs.confd)
                return true;
            return false;
        };
        
        int Buffer::readInSocket()
        {
            char extraBuffer[65535];
            bzero(extraBuffer,65535);
            int n = ::read(confd,extraBuffer,65535);

            if(n > 0)
                appendInBuffer(extraBuffer);
            
            return n;
        };

        int Buffer::writeInSocket()
        {
            int n = ::write(confd,buffer.c_str(),buffer.size()); 
            
            if(n > 0)
                buffer.erase(0,n);
            else if(n == 0)
            {}
            else
            {
                unet::handleError(errno);
            }

            return n;
        }
        
        void Buffer::appendInBuffer(const char* message)
        {
            buffer.append(message);
            buffer.append("\r\n");
        }

        void Buffer::appendInBuffer(const std::string& message)
        {
            buffer.append(message);
            buffer.append("\r\n");
        }

        void Buffer::getCompleteMessageInBuffer(std::string& message)
        {
            size_t index = buffer.find_first_of("\r\n");
            message.clear();

            if(index != std::string::npos)
            {
                message.append(buffer,0,index); 
                buffer.erase(0,index+2);
            }
        }

        void Buffer::sendFile(const char* filename)
        {
            int fd = ::open(filename,file::READ);
            if(fd < 0)
                unet::handleError(errno);
            
            struct stat statBuf;
            if(fstat(fd,&statBuf) < 0)
                unet::handleError(errno);

/* 发送文件的通信协议设计           
            char* index = strrchr(const_cast<char*>(filename),'/');
            char* buf = NULL;
            if(index != NULL)
            {
                ++index;
                buf = index;
            }
            
            appendInBuffer(buf);
*/            
            
            sendfile(confd,fd,0,statBuf.st_size);
            file::writen(confd,"\r\n",2);
        }

        void Buffer::sendFile(const std::string& filename)
        {
            sendFile(filename.c_str());
        }
        
        void Buffer::sendFile(const file::File& lhs)
        {
            sendFile(lhs.getGlobalFilename().c_str());
        }

        void Buffer::recvFile(const char* filename)
        {
            int fd = ::open(filename,file::N_WRITE); 
            size_t index = 0;

            char buf[4096];
            bzero(buf,4096);

            while(1)
            {
                index = ::read(confd,buf,4096); 
                
                if(index == 4096)
                {
                    file::writen(fd,buf,4096);
                    bzero(buf,4096);
                    continue;
                }
                else
                {
                    file::writen(fd,buf,index);
                    break;
                }
            }           
        }

        void Buffer::recvFile(const std::string& filename)
        {
            recvFile(filename.c_str());
        }

        void Buffer::recvFile(const file::File& lhs)
        {
            recvFile(lhs.getGlobalFilename().c_str());
        }
    }
}
