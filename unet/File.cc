/*************************************************************************
	> File Name: File.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月31日 星期五 18时24分30秒
 ************************************************************************/

#include"File.h"
#include"error.h"
#include<sys/stat.h>

namespace unet
{
    namespace file
    { 
        File::File(const char* filename_,int type_) :       opened(false),
            g_filename(filename_),
            type(type_)
        {
            init();
        }

        File::File(const std::string& filename_,int type_) : opened(false),
        g_filename(filename_),
        type(type_)
        {
            init();
        }
             
        void File::init()
        {
            char* index = strrchr(const_cast<char*>(g_filename.c_str()),'/');
            char buf[64];
            bzero(buf,64);
            if(index != NULL)
            {
                ++index;
                filename = index;
            }
            else
            {
                perror("请输入全局地址");
                exit(1);
            }
            
            fd = switchOperatorType(type);     

            struct stat statBuf;
            if(fstat(fd,&statBuf) < 0)
                unet::handleError(errno);

            fileSize = statBuf.st_size;
        }
        
        int File::switchOperatorType(int type_)
        {
            int fd = ::open(g_filename.c_str(),type_);
            
            if(fd < 0)
                unet::handleError(errno);
            return fd;
        }
        
        File::File(File&& lhs) : 
            opened(false),
            filename(std::move(lhs.filename)),
            g_filename(std::move(lhs.g_filename)),
            type(lhs.type),
            fileSize(lhs.fileSize)
        {
            fd = switchOperatorType(type);
            opened = true;
            
            if(::close(lhs.fd) < 0)
                unet::handleError(errno);
            lhs.opened = false;
        }
        
        File& File::operator=(File&& lhs)
        {
            if(*this == lhs)
                return *this;

            if(::close(this->fd) < 0)
                unet::handleError(errno);
            this->opened = false;
            
            if(::close(lhs.fd) < 0)
                unet::handleError(errno);
            this->opened = false;

            this->filename = std::move(lhs.filename);
            this->g_filename = std::move(lhs.g_filename);
            this->fd = switchOperatorType(this->type);
            this->opened = true;
            this->fileSize = lhs.fileSize;

            return *this;
        }
        
        File::~File()
        {
            if(opened)
            {
                if(::close(fd) < 0)
                    unet::handleError(errno);
            }
        }
       
        int readn(int fd,char* cptr,size_t nbytes)
        {
            int nleft,nread;
            
            int readsize = 0;
            nleft = nbytes;

            while(nleft > 0)
            {
                if((nread=read(fd,cptr,nleft)) < 0)
                {
                    if(errno == EINTR)
                        nread = 0;
                    else
                    {
                        perror("readn error!\n");
                        exit(0);
                    }
                }
                else if(nread == 0)
                    break;
                nleft -= nread;
                cptr += nread;
            }

            readsize = nbytes - nleft;
            return readsize;
        }
        
        int readn(int fd,std::string& buf,size_t nbytes)
        {
            char cptr[nbytes];
            bzero(cptr,nbytes);
            
            int nleft,nread;
            int readsize = 0;
            nleft = nbytes;

            while(nleft > 0)
            {
                if((nread=read(fd,cptr,nleft)) < 0)
                {
                    if(errno == EINTR)
                        nread = 0;
                    else
                    {
                        perror("readn error!\n");
                        exit(0);
                    }
                }
                else if(nread == 0)
                    break;
                
                nleft -= nread;
            }
            
            readsize = nbytes - nleft;
            buf = cptr;
            return readsize;
        }
        
        int writen(int fd,const char* cptr,size_t nbytes)
        {
            int nleft,nwriten;
    
            nleft = nbytes;
            int writesize = 0;
            while(nleft > 0)
            {
                if((nwriten=write(fd,cptr,nleft)) <= 0)
                {
                    if(nwriten<0 && errno!=EINTR)
                    nwriten = 0;
                    else
                    {
                        perror("writen error!\n");
                        exit(0);
                    }
                }
                nleft -= nwriten;
                cptr += nwriten;
            }
            writesize  = nbytes - nleft;
            
            return writesize;
        }
        
        int writen(int fd,const std::string& buf)
        {
            return writen(fd,buf.c_str(),buf.size());
        }

        bool operator==(const File& lhs,const File& rhs)
        {
            if(lhs.fd == rhs.fd && lhs.g_filename == rhs.g_filename )
                return true;
            else
                return false;
        }
    }
}
