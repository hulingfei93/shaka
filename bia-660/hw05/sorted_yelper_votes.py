# -*- coding: utf-8 -*-
"""
Created on Thu Oct 08 18:56:24 2015

@author: Lingfei Hu
"""
import urllib2, re
import sys

try:
    browser = urllib2.build_opener()
    browser.addheaders = [('User-agent', 'Mozilla/5.0')]

    fileReader = open('input.txt')
    fileWriter = open('output.txt', 'w')
    
    user_dict = dict()
    url = fileReader.readline()
    
    print 'Donwloading: ', url
    html = browser.open(url).read()
    
    review_list_match = re.search('<ul class="ylist ylist-bordered reviews".*?>[\w\W]*?</ul>\s*</div>\s*(?=<div class="review-pager">)', html)
    
    if review_list_match:
        reviews = review_list_match.group(0).split('<div class="review review--with-sidebar"')
        
        for review in reviews:
            # user name
            name_matche = re.search('<a class="user-display-name".*?>(.*?)</a>', review);
            if not name_matche:
                continue;
                
            name = name_matche.group(1);
            
            # votes
            vote_matches = re.finditer('<li class="vote-item.*?>[\w\W]*?<span class="count">(\d+)</span>[\w\W]*?</li>', review)
            vote_sum = 0
            for vote_match in vote_matches:
                vote_sum = vote_sum + int(vote_match.group(1))
            
            # if user exists in dict
            if name in user_dict:
                    user_dict[name] = user_dict[name] + vote_sum
            # if user doesn't exist in dict
            else:
                user_dict[name] = vote_sum
            
    # sort and save
    for key in sorted(user_dict):
        fileWriter.write(key + '\t' + str(user_dict[key]) + '\n') 
    
    fileReader.close()
    fileWriter.close()
            
except:
    error_type, error_obj, error_info = sys.exc_info()
    print error_type, 'Line:', error_info.tb_lineno

