/*************************************************************************
  > File Name: Buffer.h
  > Author: uuchen
  > Mail: 1319081676@qq.com
  > Created Time: 2017年03月14日 星期二 19时28分38秒
 ************************************************************************/

#ifndef _BUFFER_H
#define _BUFFER_H

#include<string.h>
#include<sys/uio.h>
#include<malloc.h>
#include<iostream>
#include<functional>
#include"Mutex.h"
#include<sys/types.h>
#include<sys/stat.h>
//目前版本只支持固定大小的数据传输

namespace unet
{
    namespace net
    {
        class Buffer final
        {
            typedef std::function<void()> CloseCallBack;

            public:
                explicit Buffer(int fd_) : buffer(nullptr),KBufferSize(1024),level(KBufferSize/2),headindex(0),tailindex(0),key(0),fd(fd_)
            {
                buffer = static_cast<char*>(malloc(KBufferSize));
                bzero(buffer,KBufferSize);
            };

                Buffer(const Buffer& lhs) = delete;
                Buffer& operator=(const Buffer& lhs) = delete;

                ~Buffer()
                {
                    free(buffer);
                };

                //public interface
                int readInSocket();
                void writeInSocket();
                
                //通用操作
                void appendInBuffer(const char* message,int size);
                void getCompleteMessageInBuffer(char* message);
                
                //针对File
                int readInSocket(int size);
                int sendFile(const char* filename,int size=1024);
                int recvFile(const char* filename,int size=1024);
                
                int getDataSize() const 
                {return tailindex - headindex;};

                void getData() const
                {
                    printf("%s\n",buffer+headindex);
                }
        
                int getFreeSize() const 
                {return KBufferSize - tailindex;};

                bool needMove() const
                {return headindex >= level;};

                int getHeadIndex() const 
                {return headindex;};

                int getTailIndex() const 
                {return tailindex;};

                int getBufferSize() const 
                {return KBufferSize;};
               
                bool getKey() const
                {return key;};
                
                void setFd(int fd_)
                {fd = fd_;}

                int getFd()
                {return fd;};
                
                void useCloseCallBack()
                {closecallback();};

                void setHandleCloseCallBack(const CloseCallBack& cb)
                {closecallback = cb;};

            private:
                char* buffer;
                int KBufferSize;
                int level;
                int headindex;
                int tailindex;
                int key;
                int fd;
                CloseCallBack closecallback;
        };        
    }
}













#endif

