/*************************************************************************
	> File Name: Buffer.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月14日 星期二 19时28分46秒
 ************************************************************************/

#include"Buffer.h"
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include"File.h"

namespace unet
{
    namespace net
    {
        int Buffer::readInSocket()
        {
            unet::thread::MutexLockGuard guard(lock);
            
            char extrabuf[65536];
            bzero(extrabuf,65536);
            struct iovec vec[2];
            vec[0].iov_base = buffer + tailindex;
            vec[0].iov_len = getFreeSize();
            vec[1].iov_base = extrabuf;
            vec[1].iov_len = 65536;
            
            int n = ::readv(fd,vec,getFreeSize());
            printf("readv n: %d\n",n);

            if(n < 0)
            {
                perror("readv error\n");
                closecallback();
            }
            else if(n == 0)
            {
                closecallback();
            }
            else if(n < getFreeSize() && n > 0)
            {
                tailindex += n;
            }
            else
            {    
                tailindex += getFreeSize();
                int size = n - getFreeSize();
                while(size > getFreeSize())
                {
                    KBufferSize *= 2;
                    level = KBufferSize / 2;
                    buffer = static_cast<char*>(realloc(static_cast<char*>(buffer),static_cast<size_t>(KBufferSize)));
                }
                strcpy(buffer+tailindex,extrabuf);
                tailindex += size;
            }
            return n;
        }
            
        void Buffer::writeInSocket()
        {
            unet::thread::MutexLockGuard guard(lock);
            int n = ::write(fd,buffer+headindex,getDataSize());

            std::cout << "writeInSocket : " << n << std::endl;
            
            if(n > 0)
            {
                headindex += n;
                if(needMove())
                {
                    strcpy(buffer,buffer+headindex);
                    tailindex -= headindex;
                    headindex = 0;
                }
            }
            else
            {
                perror("write error!\n");
            }
        }

        //通用操作
        void Buffer::appendInBuffer(const char* message)
        {
            unet::thread::MutexLockGuard guard(lock);
            int size = strlen(message);
            if(size+2 > getFreeSize())
            {
                while(size+2 > getFreeSize())
                {
                    KBufferSize *= 2;
                    level  = KBufferSize / 2;
                    buffer = static_cast<char*>(realloc(static_cast<char*>(buffer),static_cast<size_t>(KBufferSize)));
                }
            }
            
            std::cout << "----------------------" << std::endl;
            std::cout << "appendInBuffer: " << size << std::endl;

            strncat(buffer,message,size);
            strncat(buffer,"\r\n",2);
            tailindex += size;
            tailindex += 2;
        }
        
        void Buffer::getCompleteMessageInBuffer(char* message)
        {
            unet::thread::MutexLockGuard guard(lock);
            char* ch = strstr(buffer+headindex,"\r\n");
            
            if(ch != NULL)
            {
                int size = ch - buffer - headindex;
                strncpy(message,buffer+headindex,size);
                headindex += size;
                headindex += 2;
                

                if(needMove())
                {
                    strcpy(buffer,buffer+headindex);
                    tailindex -= headindex;
                    headindex = 0;
                }
                
            }
            else
                message = nullptr;
        }

        //File操作
        int Buffer::readInSocket(int size)
        {
            if(getFreeSize() < size)
            {
                strcpy(buffer,buffer+headindex);
                tailindex -= headindex;
                headindex = 0;
            }

            while(getFreeSize() < size)
            {
                KBufferSize *= 2;
                level  = KBufferSize / 2;
                buffer = static_cast<char*>(realloc(static_cast<char*>(buffer),static_cast<size_t>(KBufferSize)));
            }

            int n = ::read(fd,buffer+tailindex,size); 

            if(n == 0)
                ::close(fd);

            tailindex += 1024; 

            return n;
        }

        int Buffer::sendFile(const char* filename,int size)
        {
            File uuchen(filename);
            int n;

            while(1)
            {
                if(getFreeSize() < size)
                {
                    strcpy(buffer,buffer+headindex);
                    tailindex -= headindex;
                    headindex = 0;
                }

                while(getFreeSize() < size)
                {
                    KBufferSize *= 2;
                    level  = KBufferSize / 2;
                    buffer = static_cast<char*>(realloc(static_cast<char*>(buffer),static_cast<size_t>(KBufferSize)));
                }

                uuchen.readn(buffer+tailindex,1024);
                n = uuchen.getReadSize();
                tailindex += n;
                
                if(n > 0)
                    writeInSocket();
                else if(n == 0)
                    return 1;
                else
                    break;
            }
            return -1;
        }

        int Buffer::recvFile(const char* filename,int size)
        {
            File chenuu(filename);
            int n;

            while(1)
            {
                n = readInSocket(size);
                chenuu.writen(buffer+headindex,n);
                headindex += n;
                  
                if(n > 0)
                    continue;
                else if(n == 0)
                {
                    ::chmod(filename,S_IRUSR|S_IWUSR);
                    return 1;
                }
                else
                    break;
            }
            return -1;
        }
    }
}
