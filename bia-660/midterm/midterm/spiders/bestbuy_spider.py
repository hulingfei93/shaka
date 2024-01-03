# -*- coding: utf-8 -*-
"""
Created on Tue Oct 06 21:34:29 2015

@author: Lingfei Hu
"""


import scrapy
from midterm.items import ReviewItem
import re

class bestbuy_spider(scrapy.Spider):
    name = "bestbuy"
    allowed_domains = ["bestbuy.com", "bestbuy.ugc.bazaarvoice.com"]
    
    start_urls = ["http://www.bestbuy.com/site/searchpage.jsp?st=tv&id=pcat17071&nrp=15&cp=1"]
    review_url_pattern = "http://bestbuy.ugc.bazaarvoice.com/3545w/{0}/reviews.djs?format=embeddedhtml"
    
    
    def parse(self, response):
        # search list
        tv_urls = response.css('.list-item').css('.sku-title').xpath('.//a/@href').extract()
        for tv_url in tv_urls:
            skuid_match = re.search('skuId=(\d+)', tv_url)
            if skuid_match:
                yield scrapy.Request(self.review_url_pattern.format(skuid_match.group(1)), callback=self.parse_tv_review)

        # next page
        next_search_nums = response.css('.pager-next').xpath('./@data-page-no').extract()
        if len(next_search_nums) > 0:
            next_search_url = re.sub('cp=\d+', 'cp=' + str(next_search_nums[0]), response.url)
            yield scrapy.Request(next_search_url, callback=self.parse)
        
        

    def parse_tv_review(self, response):
        reviews_matches = re.finditer('<div id=\\\\"BVRRDisplayContentReviewID_\d+\\\\".*?>[\w\W]*?<div class=\\\\"BVRRSeparator BVRRSeparatorContentBodyBottom\\\\">[\w\W]*?<\\\\/div>[\w\W]*?<\\\\/div>', response.body)
        
        for review_match in reviews_matches:
            review = review_match.group(0)
            review_item = ReviewItem()
            
            #domain
            review_item["domain"] = "bestbuy.com"
            
            # review text
            review_item["text"] = ""
            title_match = re.search('<span property=\\\\"v:summary\\\\" class=\\\\"BVRRValue BVRRReviewTitle\\\\">([\w\W]*?)<\\\\/span>', review)
            content_match = re.search('<span class=\\\\"BVRRReviewText\\\\">([\w\W]*?)<\\\\/span>', review)
            if title_match:
                review_item["text"] = review_item["text"] + title_match.group(1)
            if content_match:
                review_item["text"] = review_item["text"] + ' ' + re.sub('[\t\n]', '', content_match.group(1))

            # rating
            rating_match = re.search('<span property=\\\\"v:value\\\\" class=\\\\"BVRRNumber BVRRRatingNumber\\\\">(\d+)<\\\\/span>', review)
            if rating_match:
                review_item["rating"] = rating_match.group(1)

            # date
            date_match = re.search('<span property=\\\\"v:dtreviewed\\\\" content=\\\\"([\d-]+)\\\\" class=\\\\"BVRRValue BVRRReviewDate\\\\">', review)
            if date_match:
                review_item["date"] = date_match.group(1)
                
            yield review_item
            