/*************************************************************************
	> File Name: SerialPort.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月29日 星期一 18时01分30秒
 ************************************************************************/

#include"SerialPort.h"

#include<string.h>
#include<fcntl.h>
#include<termios.h>
#include<unistd.h>

namespace serial
{
    SerialPort::SerialPort(const char* filename_) : isOpened(false),g_filename(filename_)
    {
        init();
    }

    SerialPort::SerialPort(const std::string& filename_) : isOpened(false),g_filename(filename_)
    {
        init();
    }
    
    SerialPort::SerialPort(const SerialPort& lhs) : fd(dup(lhs.fd)),isOpened(true),filename(lhs.filename),g_filename(lhs.g_filename),readCallBack(lhs.readCallBack)
    {};

    SerialPort::SerialPort(SerialPort&& lhs) : fd(dup(lhs.fd)),isOpened(true),filename(std::move(lhs.filename)),g_filename(std::move(lhs.g_filename)),readCallBack(lhs.readCallBack)
    {};
    
    SerialPort& SerialPort::operator=(const SerialPort& lhs)
    {
        if(*this == lhs)
            return *this;
        
        ::close(fd);
        isOpened = false;
        
        fd = dup(lhs.fd);
        isOpened = true;
        filename = lhs.filename;
        g_filename = lhs.g_filename;
        readCallBack = lhs.readCallBack;
        return *this;
    }

    SerialPort& SerialPort::operator=(SerialPort&& lhs)
    {
        if(*this == lhs)
            return *this;
        
        ::close(fd);
        isOpened = false;

        fd = dup(lhs.fd);
        isOpened = true;
        filename = std::move(lhs.filename);
        g_filename = std::move(lhs.g_filename);
        readCallBack = std::move(lhs.readCallBack);
        return *this;
    }

    SerialPort::~SerialPort()
    {
        if(isOpened)
            ::close(fd);
    }
    
    void SerialPort::swap(SerialPort& lhs)
    {
        std::swap(fd,lhs.fd);
        std::swap(isOpened,lhs.isOpened);
        std::swap(filename,lhs.filename);
        std::swap(g_filename,lhs.g_filename);
        std::swap(readCallBack,lhs.readCallBack);
    }

    void SerialPort::init()
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
            perror("输入地址错误\n");
            exit(1);
        }
        
        fd = ::open(g_filename.c_str(),O_RDWR|O_NOCTTY|O_NONBLOCK);
        if(fd < 0)
        {
            perror("Open fd error!\n");
            exit(1);
        }
        
        setOption(9600,1,0,1);
    }

    void SerialPort::setOption(int nSpeed,int nBits,int nEvent,int nStop)
    {
        struct termios newtio,oldtio;
        
        bzero(&oldtio,sizeof(oldtio));
        if(::tcgetattr(fd,&oldtio) < 0)
        {
            perror("tcgetattr error!\n");
            exit(1);
        }
        
        //设置字符大小
        bzero(&newtio,sizeof(newtio));
        newtio.c_cflag |= CLOCAL | CREAD;
        newtio.c_cflag &= ~CSIZE;
        
        switch(nBits)
        {//设置停止位
            case 7:
                newtio.c_cflag |= CS7;
                break;
            case 8:
                newtio.c_cflag |= CS8;
                break;
            default:
                break;
        }

        switch(nEvent)
        {//设置奇偶校验位
            case 'o':
            case 'O':
                newtio.c_cflag |= PARENB;
                newtio.c_cflag |= PARODD;
                newtio.c_iflag |= (INPCK | ISTRIP);
                break;
            case 'e':
            case 'E':
                newtio.c_iflag |= (INPCK | ISTRIP);
                newtio.c_cflag |= PARENB;
                newtio.c_cflag &= ~PARODD;
                break;
            case 'n':
            case 'N':
                newtio.c_cflag &= ~PARENB;
                break;
            default:
                break;
        }

        switch(nSpeed)
        {//设置比特率
            case 2400:
                cfsetispeed(&newtio,B2400);
                cfsetospeed(&newtio,B2400);
                break;
            case 4800:
                cfsetispeed(&newtio,B4800);
                cfsetospeed(&newtio,B4800);
                break;
            case 9600:
                cfsetispeed(&newtio,B9600);
                cfsetospeed(&newtio,B9600);
                break;
            case 115200:
                cfsetispeed(&newtio,B115200);
                cfsetospeed(&newtio,B115200);
                break;
            case 460800:
                cfsetispeed(&newtio,B460800);
                cfsetospeed(&newtio,B460800);
                break;
            default:
                cfsetispeed(&newtio,B9600);
                cfsetospeed(&newtio,B9600);
                break;
        }

        //设置停止位
        if(nStop == 1)
            newtio.c_cflag &= ~CSTOPB;
        else if(nStop == 2)
            newtio.c_cflag |= CSTOPB;
        else
            ;
        
        //设置等待时间和最小接受字符
        newtio.c_cc[VTIME] = 0;
        newtio.c_cc[VMIN] = 0;
        
        //处理未接受的字符
        tcflush(fd,TCIFLUSH);
        
        if((tcsetattr(fd,TCSANOW,&newtio)) < 0)
            perror("tcsetattr error\n");
    }

    void SerialPort::read()
    {
        char buf[8];
        if(::read(fd,buf,8) < 0)
        {
            perror("read error\n");
            exit(1);
        }
        readCallBack(std::string(buf));
    }

    void SerialPort::write(char c)
    {
         if(::write(fd,&c,1) < 0)
         {
             perror("write error\n");
             exit(1);
         }
    }
    
    bool operator==(const SerialPort& lhs,const SerialPort& rhs)
    {
        return (lhs.fd == rhs.fd && lhs.g_filename == rhs.g_filename);
    }
}

