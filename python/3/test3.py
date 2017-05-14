#!/usr/bin/env python3
# coding=utf-8

things = ["uuchen","chenuu","wang"]

for n in range(len(things)):
    things[n] = things[n].title()
    if(n == (len(things)-1)):
        things[n] = things[n][::-1].title()



print(things)



