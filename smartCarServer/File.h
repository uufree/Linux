/*************************************************************************
	> File Name: File.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月27日 星期一 18时04分42秒
 ************************************************************************/
/*设计理念：使用封装解决琐碎的问题
 * 
 * 1. O_WRITE: write
 * 2. O_C_WRITE: clear write
 * 3. O_READ: read
 * 4. N_WRITE: new write
 * 
 * 5.使用分离式构造的方式完成基本的构造
 */

#ifndef _FILE_H
#define _FILE_H

#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<string>
#include<string.h>

namespace unet
{
    namespace file
    {
        static const int WRITE = O_WRONLY|O_APPEND|O_EXCL;
        static const int C_WRITE = O_WRONLY|O_TRUNC|O_APPEND|O_CREAT;
        static const int READ = O_RDONLY|O_EXCL;
        static const int N_WRITE = O_WRONLY|O_CREAT|O_APPEND; 

        class File final
        {
            friend bool operator==(const File& lhs,const File& rhs);

             public:
                explicit File(const char* filename_,int type_);
                explicit File(const std::string& filename_,int type_);

                File(const File& lhs) = delete;
                File(File&& lhs);
                File& operator=(const File& lhs) = delete;
                File& operator=(File&& lhs);
                ~File();  

                const std::string& getFilename() const
                {return filename;};

                const std::string& getGlobalFilename() const
                {return g_filename;};

                int getFd() const
                {return fd;};
                
                bool isOpened() const
                {return opened;};    
                
                int getFileSize()
                {return fileSize;};

            private:
                void init();
                int switchOperatorType(int type_);

            private:
                int fd;
                bool opened;
                std::string filename;
                std::string g_filename;
                int type;
                int fileSize;
        };
        
        bool operator==(const File& lhs,const File& rhs);

        int readn(int fd,char* cptr,size_t nbytes);
        int readn(int fd,std::string& buf,size_t nbytes);
        
        int writen(int fd,const char* cptr,size_t nbytes);
        int writen(int fd,const std::string& buf);
        
    }
}
            
#endif



