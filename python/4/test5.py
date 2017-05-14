#!/usr/bin/env python3
# coding=utf-8

def returnList():
    return [1,2,3,4]

def getOdds():
    yield [number for number in range(0,9) if number%2 != 0]

lists = getOdds()

for number in lists:
    print(number)

for number in lists:
    print(number)
