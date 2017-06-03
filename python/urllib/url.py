#!/usr/bin/env python3
# coding=utf-8

import urllib.request
import os

response = urllib.request.urlopen("http://wthrcdn.etouch.cn/weather_mini?citykey=101010100")
page = response.read()
print(page)
fp = open('/home/uuchen/weather.gzip','wb')
fp.write(page)
fp.close()
