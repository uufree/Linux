#!/usr/bin/env python3
# coding=utf-8

import requests

url = "http://www.baidu.com"
resp = requests.get(url)
print(resp)
print(resp.text)

