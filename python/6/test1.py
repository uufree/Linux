#!/usr/bin/env python3
# coding=utf-8

class Count:
    count = 0;
    def __init__(self):
        Count.count += 1

    @classmethod
    def kids(cls):
        print("have ",cls.count,"objects")



a = Count()
b = Count()
c = Count()

Count.kids()
