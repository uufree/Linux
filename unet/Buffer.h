/*************************************************************************
  > File Name: Buffer.h
  > Author: uuchen
  > Mail: 1319081676@qq.com
  > Created Time: 2017年03月14日 星期二 19时28分38秒
 ************************************************************************/

/* 设计理念:使用STL容器管理内存，避免使用C-Style风格，接受性能损失
 
 * 1.一个Buffer只做一样工作，读或者写。
 * 2.异步会对Buffer加锁 
 * 3.内部紧耦合，尽可能少的向外部暴露接口
 * 4.尽量节省空间（挪移），此举会带来性能损失
 * 5.根据需要使用STL自行扩展
 */

#ifndef _BUFFER_H
#define _BUFFER_H

#include<string>
#include"File.h"

namespace unet
{
    namespace net
    {
        class File;

        class Buffer final
        {
            friend bool operator==(const Buffer& lhs,const Buffer& rhs);
            
            public:
                explicit Buffer(int fd_);
                Buffer(const Buffer& lhs) = delete;
                Buffer(Buffer&& lhs);
                Buffer& operator=(const Buffer& lhs) = delete;
                Buffer& operator=(Buffer&& lhs);
                ~Buffer();    
                void swap(Buffer& lhs);

                int readInSocket();
                int writeInSocket();
                void appendInBuffer(const char* message);
                void appendInBuffer(const std::string& message);
                void getCompleteMessageInBuffer(std::string& message);
                
                //针对File
                void sendFile(const char* filename);
                void sendFile(const std::string& filename); 
                void sendFile(const file::File& lhs);
                void recvFile(const char* filename);
                void recvFile(const std::string& filename);
                void recvFile(const file::File& lhs);

            private:
                int confd;
                std::string buffer;
        };      

        bool operator==(const Buffer& lhs,const Buffer& rhs);
    }
}


#endif

