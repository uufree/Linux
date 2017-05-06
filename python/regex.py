#!/usr/bin/env python3
# coding=utf-8

import re
phoneNumRegex = re.compile("\d\d\d-\d\d\d\d-\d\d\d")
mo = phoneNumRegex.search("my number is 333-3333-333")
print(mo.group())

