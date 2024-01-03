# -*- coding: utf-8 -*-
"""
Created on Thu Oct 01 20:26:46 2015

@author: Lingfei Hu
"""

import urllib2,re
from urlparse import urlparse
import time

browser=urllib2.build_opener()
browser.addheaders=[('User-agent', 'Mozilla/5.0')]

fileReader=open('input.txt')
fileWriter=open('yelp_counts.txt', 'w')

for url in fileReader:
    url = url.strip()
    parsed_uri = urlparse(url)
    domain = '{uri.scheme}://{uri.netloc}/'.format(uri=parsed_uri)    
    useId = re.search('userid=(.*)', url).group(1)
    
    print 'Donwloading: ', url
    html=browser.open(url).read()
    
    pageAmount = 1
    rank1 = 0
    rank2 = 0
    rank3 = 0
    rank4 = 0

    # parse info on page 1
    matches = re.finditer('<span class="business-attribute price-range">(.*?)</span>', html)
    for match in matches:
        price = match.group(1).strip()
        if price == '$':
            rank1 = rank1 + 1
        elif price == '$$':
            rank2 = rank2 + 1
        elif price == '$$$':
            rank3 = rank3 + 1
        elif price == '$$$$':
            rank4 = rank4 + 1
        
    moreMatch = re.search('<div class="pagination-block">[\w\W]*?<a href="(.*?)">[\w\W]*?</a>[\w\W]*?</div>', html)
    
    if moreMatch:
        href = moreMatch.group(1)
        moreUrl = domain + href
        time.sleep(1)
        print 'Donwloading: ', moreUrl
        html = browser.open(moreUrl).read()
        pageCountMatch = re.search('<div class="page-of-pages arrange_unit arrange_unit--fill">[\w\W]*?Page[\w\W]*?\d+[\w\W]*?of[\w\W]*?(\d+)[\w\W]*?</div>', html)
        
        if pageCountMatch:
            pageCount = pageCountMatch.group(1)
            if (pageCount > 1):
                for pageNum in range(2,int(pageCount) + 1):
                    pageUrl = domain + 'user_details_reviews_self?rec_pagestart=' + str((pageNum - 1) * 10) + '&userid=' + useId
                    time.sleep(1)
                    print 'Donwloading: ', pageUrl
                    html = browser.open(pageUrl).read()
                    pageAmount = pageAmount + 1
                    matches = re.finditer('<span class="business-attribute price-range">(.*?)</span>', html)
                    for match in matches:
                        price = match.group(1).strip()
                        if price == '$':
                            rank1 = rank1 + 1
                        elif price == '$$':
                            rank2 = rank2 + 1
                        elif price == '$$$':
                            rank3 = rank3 + 1
                        elif price == '$$$$':
                            rank4 = rank4 + 1
                    
        
    fileWriter.write(str(pageAmount) + '\n')
    fileWriter.write(str(rank1) + '\n')
    fileWriter.write(str(rank2) + '\n')
    fileWriter.write(str(rank3) + '\n')
    fileWriter.write(str(rank4) + '\n')

    
     
fileReader.close()
fileWriter.close()