# -*- coding: utf-8 -*-
"""
Created on Thu Sep 24 20:41:41 2015

@author: Lingfei Hu
"""

import urllib2,re

browser=urllib2.build_opener()
browser.addheaders=[('User-agent', 'Mozilla/5.0')]

fileReader=open('input.txt')
fileWriter=open('stats.txt', 'w')
for name in fileReader:
    name=name.strip()
    
    url = 'https://www.freelancer.com/u/' + name + '.html'
    
    print 'Donwloading: ', url
    
    html=browser.open(url).read()
    
    matches = re.finditer('<span class="item-stats-value">(\d+?)%</span>',html)
    
    result = name
    for match in matches:
        result = result + ';' + match.group(1);
    
    result = result + '\n'
    
    fileWriter.write(result)
   
    
     
fileReader.close()
fileWriter.close()