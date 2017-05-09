/*************************************************************************
	> File Name: File.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月31日 星期五 18时24分30秒
 ************************************************************************/

#include"File.h"

namespace unet
{
    File::File(const char* filename_)
    {
        char* index = strrchr(const_cast<char*>(filename_),'/');
        char buf[64];
        bzero(buf,64);
        if(index != NULL)
        {
            ++index;
            filename = index;

            int size = strlen(filename_) - strlen(index);
            strncpy(buf,filename_,size);
        }
        else
        {
            filename = filename_;
            buf[0] = '/';
        }
        
        bool isoldfile = false;
        DIR* dp;
        struct dirent* dirp;

        if((dp=opendir(buf)) != NULL)
        {
            while((dirp=readdir(dp)) != NULL)
            {
                if((strcmp(dirp->d_name,index)) == 0)
                    isoldfile = true;
            }
        }

        
        if(!isoldfile)
            fd = ::open(filename_,O_RDWR|O_APPEND|O_CLOEXEC|O_CREAT|O_EXCL|O_TRUNC);
        else
            fd = ::open(filename_,O_RDWR|O_APPEND);
        
        assert(fd >= 0);
        
        opened = true;
        closed = false;

    }
    
    void File::readn(char* cptr,size_t nbytes)
    {
        int nleft,nread;
        
        readsize = 0;
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
    }
            
    void File::writen(char* cptr,size_t nbytes)
    {
        int nleft,nwriten;
    
        nleft = nbytes;
        
        writesize = 0;
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
    }

    Directory::Directory(const char* path) : directorypath(path),
        lock()
    {
        directorybuffer = new char[4096];
        update();
    }

    Directory::~Directory()
    {
        delete [] directorybuffer;
    }
    
    char* Directory::getDirBuffer() const 
    {
        return directorybuffer;
    }

    void Directory::update()
    {
        thread::MutexLockGuard guard(lock);
        DIR* dp = NULL;
        struct dirent* drip = NULL;

        assert((dp=::opendir(directorypath.c_str())) != NULL);

        while((drip=readdir(dp)) != NULL)
        {
            if((strcmp(drip->d_name,".")==0) || (strcmp(drip->d_name,"..")==0))
                continue;
            
            directorylist.push_back(drip->d_name);
        }
        memset(directorybuffer,'\0',4096);

        for(auto iter=directorylist.cbegin();iter!=directorylist.cend();++iter)
        {
            strcat(directorybuffer,iter->c_str());
            strcat(directorybuffer,"\n");
        }
    }

    void Directory::addInDirectoryList(const char* filename)
    {
        thread::MutexLockGuard guard(lock);
        directorylist.push_back(filename);
        strcat(directorybuffer,filename);
        strcat(directorybuffer,"\n");
    }
    
    bool Directory::inDirectoryList(const char* filename)
    {
        auto iter = find(directorylist.begin(),directorylist.end(),filename);
        return iter != directorylist.end();

    }
}
