#!/usr/bin/env python3
# coding=utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )
from bs4 import BeautifulSoup
import csv
import urllib


def get_html(url):
    html = urllib.urlopen(url)
    return html.read()

def get_data(html_text):
    final = []
    bs = BeautifulSoup(html_text, "html.parser")
    body = bs.body
    data = body.find('div', {'id': '7d'})
    ul = data.find('ul')
    li = ul.find_all('li')

    for day in li:
        temp = []
        date = day.find('h1').string
        temp.append(date)
        inf = day.find_all('p')
        temp.append(inf[0].string,)
        if inf[1].find('span') is None:
            temperature_highest = None
        else:
            temperature_highest = inf[1].find('span').string
            temperature_highest = temperature_highest.replace('C', '')
        temperature_lowest = inf[1].find('i').string
        temperature_lowest = temperature_lowest.replace('C', '')
        temp.append(temperature_highest)
        temp.append(temperature_lowest)
        final.append(temp)

    return final


def write_data(data, name):
    file_name = name
    with  open(file_name, 'a') as f:
            f_csv = csv.writer(f)
            f_csv.writerows(data)
            print(data)

if __name__ == '__main__':
    html_doc = get_html('http://www.weather.com.cn/weather/101190401.shtml')
    result = get_data(html_doc)
    write_data(result, 'weather.csv')
