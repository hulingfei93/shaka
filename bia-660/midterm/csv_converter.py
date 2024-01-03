# -*- coding: utf-8 -*-
"""
Created on Thu Oct 08 14:32:11 2015

@author: Lingfei Hu
"""

import csv
import sys

input_file = open(sys.argv[1], 'rb')
output_file = open(sys.argv[2], 'wb')

reader = csv.reader(input_file, delimiter=',')
writer = csv.writer(output_file, delimiter='\t')

for date, rating, domain, text in reader:
    writer.writerow([domain, text, rating, date])
    
input_file.close()
output_file.close()