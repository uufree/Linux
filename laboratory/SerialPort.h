/*************************************************************************
	> File Name: SerialPort.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月29日 星期一 18时01分19秒
 ************************************************************************/

#ifndef _SERIALPORT_H
#define _SERIALPORT_H

#include<string>
#include<functional>
#include<iostream>

namespace serial
{
    class SerialPort final
    {
        typedef std::function<void(std::string)> ReadCallBack;

        friend bool operator==(const SerialPort& lhs,const SerialPort& rhs);

        public:
            explicit SerialPort(const char* filename_);
            explicit SerialPort(const std::string& filename_);
            
            SerialPort(const SerialPort& lhs);
            SerialPort(SerialPort&& lhs);
            SerialPort& operator=(const SerialPort& lhs);
            SerialPort& operator=(SerialPort&& lhs);
            ~SerialPort();
            void swap(SerialPort& lhs);

            void read();
            void write(char c);
            
            void printMessage()
            {
                std::cout << "filename: " << filename << std::endl;
                std::cout << "g_filename: " << g_filename << std::endl;
                std::cout << "fd: " << fd << std::endl;
            }
            
            void setReadCallBack(const ReadCallBack& lhs)
            {readCallBack = lhs;};

        private:
            void init();
            void setOption(int nSpeed,int nBits,int nEvent,int nstop);

        private:
            int fd;
            bool isOpened;
            std::string filename;
            std::string g_filename;
            ReadCallBack readCallBack;
    };

}


#endif
