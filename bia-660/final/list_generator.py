# -*- coding: utf-8 -*-
"""
Created on Fri Nov 20 13:37:33 2015

@author: Lingfei Hu
"""

import sys

input_file = open(sys.argv[1])
output_file = open(sys.argv[2], 'w')

word_list = []

for line in input_file:
    line = line.strip()
    if line == '':
        continue
    if line.startswith(';'):
        continue
    word_list.append(line)
    
word_list_str = '\', \''.join(word_list)

output_file.write(word_list_str)
    
input_file.close()
output_file.close()