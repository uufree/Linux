/*************************************************************************
	> File Name: SerialPort.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月29日 星期一 18时01分19秒
 ************************************************************************/

#ifndef _SERIALPORT_H
#define _SERIALPORT_H

#include<iostream>
#include<string>

namespace serial
{
    class SerialPort final
    {
        friend bool operator==(const SerialPort& lhs,const SerialPort& rhs);

        public:
            explicit SerialPort(const char* filename_);
            explicit SerialPort(const std::string& filename_);
            
            SerialPort(const SerialPort& lhs);
            SerialPort(SerialPort&& lhs);
            SerialPort& operator=(const SerialPort& lhs);
            SerialPort& operator=(SerialPort&& lhs);
            ~SerialPort();

            void init();
            void read();
            void write();

        private:
            int fd;
            bool isOpened;
            std::string filename;
            std::string g_filename;
    };
}




#endif
