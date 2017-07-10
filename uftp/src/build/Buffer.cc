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

namespace unet
{
    namespace net
    {
        Buffer::Buffer(int fd_,int bufferSize_) : 
            fd(fd_),
            buffer(),
            bufferSize(bufferSize_),
            dataSize(0),
            dataIndex(0)
        {
            buffer.reserve(bufferSize);
        };

        Buffer::Buffer(Buffer&& lhs) : 
            fd(lhs.fd),
            buffer(std::move(lhs.buffer)),
            bufferSize(lhs.bufferSize),
            dataSize(lhs.dataSize),
            dataIndex(lhs.dataIndex)
        {};

        Buffer& Buffer::operator=(Buffer&& lhs)
        {
            fd = lhs.fd;
            buffer = std::move(lhs.fd);
            bufferSize = lhs.bufferSize;
            dataSize = lhs.dataSize;
            dataIndex = lhs.dataIndex;

            return *this;
        }
        
        Buffer::~Buffer()
        {};

        void Buffer::swap(Buffer& lhs)
        {
            std::swap(fd,lhs.fd);
            std::swap(buffer,lhs.buffer);
            std::swap(bufferSize,lhs.bufferSize);
            std::swap(dataSize,lhs.dataSize);
            std::swap(dataIndex,lhs.dataIndex);
        }
        
        bool operator==(const Buffer& lhs,const Buffer& rhs)
        {
            if(lhs.fd == rhs.fd)
                return true;
            return false;
        };
        
        int Buffer::getFreeSize(int size) const
        {
            /* 0:freeSize足够但是末尾空间不够
             * 1:freeSize足够并且末尾空间也足够
             *-1:freeSize不够
             * */
            
            int asize = bufferSize - dataSize;
            int bsize = bufferSize - dataSize - dataIndex;

            if(asize >= size && bsize <= size)
                return 0;//不重新分配空间+移动元素
                    
            if(asize >= size && bsize >= size)
                return 1;//直接插入

            if(asize < size)
                return -1;//增大空间+移动元素

            return -1;
        };

        bool Buffer::needToMove() const
        {
            return dataIndex >= (bufferSize/2);
        }

        void Buffer::handleBufferSpace(int size,const std::string& str)
        {//只修改bufferSize,其余不管
            int n = getFreeSize(size);
            
        read:
            switch (n)
            {
                case -1:
                {
                    while((n=getFreeSize(size)) == -1)
                    {
                        bufferSize *= 2;
                        buffer.reserve(bufferSize);
                    }
                    goto read;
                }   
                case 0:
                {
                    std::string str;
                    str.reserve(bufferSize);
                    str.insert(0,buffer,dataIndex,dataSize);
                    buffer.swap(str);
                    
                    dataIndex = 0;
                }
                case 1:
                {
                    buffer.append(str);
                    break;
                }
            }

            dataSize += size;
        }

        void Buffer::handleBufferSpace(int size)
        {
            dataSize -= size;
            dataIndex += size;
            
            if(needToMove())
            {
                bufferSize /= 2;

                std::string str;
                str.reserve(bufferSize);
                str.insert(0,buffer,dataIndex,dataSize);
                buffer.swap(str);
            }
        }

        int Buffer::readInSocket()
        {
            char extraBuffer[65535];
            bzero(extraBuffer,65535);
            int n = ::read(fd,extraBuffer,65535);
            std::cout << "read n: " << n << std::endl;

            if(n > 0)
                handleBufferSpace(n,extraBuffer);
            else if(n == 0)
            {}
            else
                unet::handleError(errno);
            
            return n;
        };

        int Buffer::writeInSocket()
        {
            int n = ::write(fd,buffer.c_str(),buffer.size()); 
            
            if(n > 0)
                handleBufferSpace(n);
            else if(n == 0)
            {}
            else
                unet::handleError(errno);

            return n;
        }
        
        void Buffer::appendInBuffer(const char* message)
        {
            handleBufferSpace(strlen(message),message);
            handleBufferSpace(2,"\r\n");
        }

        void Buffer::appendInBuffer(const std::string& message)
        {
            handleBufferSpace(message.size(),message);
            handleBufferSpace(2,"\r\n");
        }

        void Buffer::getCompleteMessageInBuffer(std::string& message)
        {
            size_t index = buffer.find_first_of("\r\n");
            message.clear();

            if(index != std::string::npos)
                message.append(buffer,dataIndex,index); 
        }

        std::string&& Buffer::getCompleteMessageInBuffer()
        {
            size_t index = buffer.find_first_of("\r\n");
            std::string str(buffer,dataIndex,index);

            return std::move(str);
        }

        void Buffer::sendFile(const char* filename)
        {

        }

        void Buffer::sendFile(const std::string& filename)
        {
            sendFile(filename.c_str());
        }

        void Buffer::recvFile(const char* filename)
        {
        }

        void Buffer::recvFile(const std::string& filename)
        {
            recvFile(filename.c_str());
        }
    }
}
