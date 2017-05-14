#!/usr/bin/env python3
# coding=utf-8

maps = {number:number**2 for number in range(10) if number%2 == 0}
print(maps)

sets = {se for se in range(10) if se%2 == 0}
print(sets)

tuples = (number for number in range(10) if number%2 == 0)
for number in tuples:
    print(number)

for number in tuples:
    print(number)
