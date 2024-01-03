# -*- coding: utf-8 -*-

# Scrapy settings for midterm project
#
# For simplicity, this file contains only settings considered important or
# commonly used. You can find more settings consulting the documentation:
#
#     http://doc.scrapy.org/en/latest/topics/settings.html
#     http://scrapy.readthedocs.org/en/latest/topics/downloader-middleware.html
#     http://scrapy.readthedocs.org/en/latest/topics/spider-middleware.html

BOT_NAME = 'midterm'

SPIDER_MODULES = ['midterm.spiders']
NEWSPIDER_MODULE = 'midterm.spiders'

# Crawl responsibly by identifying yourself (and your website) on the user-agent
USER_AGENT = 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.118 Safari/537.36'
# RANDOMIZE_DOWNLOAD_DELAY = False
DOWNLOAD_DELAY = 10
COOKIES_ENABLES = False
CONCURRENT_REQUESTS = 1
CONCURRENT_REQUESTS_PER_DOMAIN = 1

CLOSESPIDER_ITEMCOUNT = 1000
DEPTH_PRIORITY = 10
CONCURRENT_ITEMS = 10

