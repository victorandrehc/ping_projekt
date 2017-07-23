#!/usr/bin/env python
import urllib2
url="https://script.google.com/macros/s/AKfycbyhGjBXVw6pPQhnUutMd0s-GjPRjVhjZuZX0paQNCrfJ2j93pF-/exec?name='PYTHON'&in='09:00:00'&out='12:00:00'"
ok="Ok"



f=[urllib2.urlopen(url).read() for i in range(10)]
print f

