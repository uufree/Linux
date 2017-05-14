#!/usr/bin/env python3
# coding=utf-8

def MyFunc(func):
    def NewFunc(a,s,d):
        print("Runing funcName: ",func.__name__)
        print("Runing funcDoc: ",func.__doc__)
        print("begin")
        func(a,s,d)
        print("end")
    return NewFunc

def fun(a,b,c):
    "I'm func!"
    print(a+b+c)

newfunc = MyFunc(fun)

newfunc(1,2,3)































