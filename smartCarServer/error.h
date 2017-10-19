/*************************************************************************
	> File Name: error.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月19日 星期五 17时31分40秒
 ************************************************************************/

#ifndef _ERROR_H
#define _ERROR_H

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

namespace unet
{
    inline void handleError(int saveErrno)
    {   
        char buf[256];
        bzero(buf,256);
        perror(strerror_r(saveErrno,buf,256));
        exit(1);
    }
}


#endif
