#!/usr/bin/env python3
# coding=utf-8

class uuchen:
    def __init__(self,name):
        self.hname = name
    
    @property
    def name(self):
        print("getName")
        return self.hname;

    @name.setter
    def name(self,name_):
        print("setName")
        self.hname = name_


cu = uuchen("uuchen")
print(cu.name)
cu.name = "chenuu"


