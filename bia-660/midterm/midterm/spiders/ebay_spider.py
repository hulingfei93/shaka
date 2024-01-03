# -*- coding: utf-8 -*-
"""
Created on Tue Oct 06 15:29:33 2015


"""

import scrapy
from midterm.items import ReviewItem
import re

class ebay_spider(scrapy.Spider):
    name = "ebay"
    allowed_domains = ["ebay.com"]  # retrict domain
    #intital url
    start_urls = [
        "http://www.ebay.com/sch/i.html?_nkw=TV",
    ]
    
    
    def parse(self, response):
        # search list
        tv_urls = response.xpath('//ul[@id="ListViewInner"]/li').css('.lvtitle').xpath('./a/@href').extract()
        for tv_url in tv_urls:
            yield scrapy.Request(tv_url, callback=self.parse_tv_detail)

        # next page
        for next_search_url in response.xpath('//table[@id="Pagination"]').css('.pagn-next').xpath('./a/@href').extract():
            yield scrapy.Request(next_search_url, callback=self.parse)
        


    def parse_tv_detail(self, response):        
        # fetch reivews
        reviews = response.xpath('//div[@id="rwid"]').css('.ebay-review-section')

        for review in reviews:
            review_item = ReviewItem()
            
            #domain
            review_item["domain"] = "ebay.com"
            
            # review text
            review_item["text"] = ""
            titles = review.css('.ebay-review-section-r').css('.review-item-title').xpath('.//text()').extract()
            contents = review.css('.ebay-review-section-r').css('.review-item-content').xpath('.//text()').extract()
            if len(titles) > 0:
                review_item["text"] = review_item["text"] + titles[0].strip()
            for content in contents:
                review_item["text"] = review_item["text"] + ' ' + re.sub('[\t\n]', '', content)

            # rating
            ratings = review.css('.ebay-star-rating').xpath('./@title').re(r'([\d\.]+).*?')
            if len(ratings) > 0:
                review_item["rating"] = ratings[0]

            # date
            dates = review.css('.review-item-date').xpath('./text()').extract()
            if len(dates) > 0:
                review_item["date"] = dates[0]
                
            yield review_item
            


   