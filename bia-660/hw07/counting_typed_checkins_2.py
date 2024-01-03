# -*- coding: utf-8 -*-
"""
Created on Tue Oct 20 18:42:03 2015

@author: Lingfei Hu
"""

import re
import urllib2
from datetime import datetime

# read single url from file
input_file = open('profile.txt')
review_url = input_file.readline().strip()
input_file.close()
print review_url

# download the page
browser=urllib2.build_opener()
browser.addheaders=[('User-agent', 'Mozilla/5.0')]

pages = []

page_html = browser.open(review_url).read()
page_count_match = re.search('<div class="page-of-pages.*?>[\w\W]*?Page \d+ of (\d+)[\w\W]*?</div>', page_html)
page_count = int(page_count_match.group(1))

for page_num in range(1, page_count + 1):
    page_url = review_url + "&rec_pagestart=" + str((page_num - 1)  * 10)
    pages.append(page_url)
    
print str(len(pages)) + 'pages'

# Dictionary that will used to count typed checkins
typed_checkins = {}
cutoff_date = datetime.strptime('7/1/2010', '%m/%d/%Y')
    
for page_url in pages:
    html=browser.open(page_url).read()
    
    # split the input html to get chunks of reviews
    review_chunks = html.split('<div class="review" data-review-id="')
    
    # remove the first chunk
    review_chunks = review_chunks[1:]
    
    # cut the needless content from the last chunk
    end_review_id = review_chunks[-1].find('<div class="pagination-block">')
    review_chunks[-1] = review_chunks[-1][:end_review_id]
    print 'Got ', len( review_chunks ), ' chunks'
    
    for chunk in review_chunks:
        #fetch the info of date
        date_match = re.search('<span class="rating-qualifier">[\w\W]*?([0-9/]+)[\w\W]*?</span>', chunk)
        if not date_match:
            continue
        date = datetime.strptime(date_match.group(1), '%m/%d/%Y')
        
        #fetch the info of checkin count
        checkin_match = re.search('<span  class="i-wrap ig-wrap-common_sprite.*?</i>.*?(\d+).*?</span>', chunk)
        if not checkin_match:
            continue
        checkin_count = int(checkin_match.group(1))
        if checkin_count == 0:
            continue
                
        
        # fetch the info of categories
        category_chunk_match = re.search( '<span class="category-str-list">([\w\W]*?)</span>', chunk)
        if not category_chunk_match:
            continue
        categories_matches = re.finditer( '<a href=".*?">(.*?)</a>' , category_chunk_match.group(1))
        for category_match in categories_matches:
            category = category_match.group(1).strip()
            if date <= cutoff_date:
                if category in typed_checkins:
                    typed_checkins[category] = typed_checkins[category] + checkin_count
                else:
                    typed_checkins[category] = checkin_count
            else:
                if checkin_count == 1:
                    continue
                if category in typed_checkins:
                    typed_checkins[category] = typed_checkins[category] + checkin_count - 1
                else:
                    typed_checkins[category] = checkin_count - 1
 

# sort and save the results
output_file = open('type_freqs.txt', 'w')
for key in sorted(typed_checkins):
    output_file.write(key + ',' + str(typed_checkins[key]) + '\n') 
output_file.close()



