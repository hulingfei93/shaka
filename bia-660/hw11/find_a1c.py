# -*- coding: utf-8 -*-
"""
Created on Sun Dec 06 14:01:49 2015

@author: Lingfei Hu
"""

import re

def is_valid_a1c(num_str):
    num = float(num_str)
    if num >= 0.0 and num <= 17.0:
        return True
    else:
        return False
        

input_file = open('data.txt')
output_file = open('output.txt','w')

now_re_list = [
                'now it is (\d+\.?\d*)',
                'now down to (\d+\.?\d*)',
                'now (\d+\.?\d*)',
                'now at (\d+\.?\d*)',
                'now is (\d+\.?\d*)',
                'now its (\d+\.?\d*)'
              ]
              
last_re_list = [
                 'last a1c was (\d+\.?\d*)',
                 'last a1c is (\d+\.?\d*)',
                 'last a1c at (\d+\.?\d*)'
               ]

std_re_list = [
                'a1c is (\d+\.?\d*)',
                'down to (\d+\.?\d*)',
                'a1c was (\d+\.?\d*)',
                'a1c at (\d+\.?\d*)',
                'a1c of (\d+\.?\d*)',
                'a1c as (\d+\.?\d*)'
              ]   

range_re_list = [
                  'a1c of (\d+\.?\d*) to (\d+\.?\d*)',
                  'a1c at (\d+\.?\d*) or (\d+\.?\d*)',
                  'between (\d+\.?\d*)\s*and\s*(\d+\.?\d*)',
                  'between (\d+\.?\d*)\s*-\s*(\d+\.?\d*)',
                  'range[d]? from (\d+\.?\d*)\s*to\s*(\d+\.?\d*)',
                  'range[d]? from (\d+\.?\d*)\s*-\s*(\d+\.?\d*)',
                  'range of (\d+\.?\d*)\s*-\s*(\d+\.?\d*)',
                  '(\d+\.?\d*)\s*-\s*(\d+\.?\d*) range'
                ]

to_re_list = [
                'from (\d+\.?\d*)\s*to\s*(\d+\.?\d*)',
                'from (\d+\.?\d*)\s*-\s*(\d+\.?\d*)',
                '(\d+\.?\d*)\s*to\s*(\d+\.?\d*)'
             ]


for raw_line in input_file:
    raw_line = raw_line.strip();
    line = raw_line.lower().replace('&hellip;', '...').replace('&mdash;', '—').replace('&amp;', '&')
    line = line.replace('hba1c', 'a1c').replace('a1cs', 'a1c').replace('alc\'s', 'a1c').replace('ha1c', 'a1c')
    line = line.replace('\'s', '').replace('%', '')
    
    # remove confused numbers
    line = re.sub('(\d+\.?\d*) pounds', '', line)
    line = re.sub('\d+ years', '', line)
    line = re.sub('\d+ months', '', line)
    line = re.sub('\d+ pts', '', line)
    line = re.sub('\d+ lbs', '', line)

    
    a1c_results = []
    
    # now related re
    for now_re in now_re_list:
        now_match = re.search(now_re, line)
        if now_match:
            a1c_str = now_match.group(1)
            if is_valid_a1c(a1c_str):
                a1c_results.append(a1c_str)
                break
        
    if len(a1c_results) > 0:
        output_file.write(' '.join(a1c_results) + '\n')
        # output_file.write(raw_line + '\t' + ' '.join(a1c_results) + '\n')
        continue;
        
    # last related re
    for last_re in last_re_list:
        last_match = re.search(last_re, line)
        if last_match:
            a1c_str = last_match.group(1)
            if is_valid_a1c(a1c_str):
                a1c_results.append(a1c_str)
                break

    if len(a1c_results) > 0:
        output_file.write(' '.join(a1c_results) + '\n')
        # output_file.write(raw_line + '\t' + ' '.join(a1c_results) + '\n')
        continue;
        
    # std related re
    for std_re in std_re_list:
        std_match = re.search(std_re, line)
        if std_match:
            a1c_str = std_match.group(1)
            if is_valid_a1c(a1c_str):
                a1c_results.append(a1c_str)
                break

    if len(a1c_results) > 0:
        output_file.write(' '.join(a1c_results) + '\n')
        # output_file.write(raw_line + '\t' + ' '.join(a1c_results) + '\n')
        continue;
        
    # range related re
    for range_re in range_re_list:
        range_match = re.search(range_re, line)
        if range_match:
            a1c_str1 = range_match.group(1)
            a1c_str2 = range_match.group(2)
            if is_valid_a1c(a1c_str1) and is_valid_a1c(a1c_str2):
                a1c_results.append(a1c_str1)
                a1c_results.append(a1c_str2)
                break

    if len(a1c_results) > 0:
        output_file.write(' '.join(a1c_results) + '\n')
        # output_file.write(raw_line + '\t' + ' '.join(a1c_results) + '\n')
        continue;
        
    # to related re
    for to_re in to_re_list:
        to_match = re.search(to_re, line)
        if to_match:
            a1c_str1 = to_match.group(1)
            a1c_str2 = to_match.group(2)
            if is_valid_a1c(a1c_str1) and is_valid_a1c(a1c_str2):
                a1c_results.append(a1c_str2)
                break

    if len(a1c_results) > 0:
        output_file.write(' '.join(a1c_results) + '\n')
        # output_file.write(raw_line + '\t' + ' '.join(a1c_results) + '\n')
        continue;
        
        
    # if there is no results by processing of above re, find the following nums after 'a1c'
    index = line.find('a1c')
    if index != -1:
        line = line[index:]
        words = line.split()
        for word in words:
            match = re.search('^(\d+\.?\d*)$', word)
            if match:
                a1c_str = match.group(1)
                if is_valid_a1c(a1c_str):
                    a1c_results.append(a1c_str)
                    break;
            
    output_file.write(' '.join(a1c_results) + '\n')
    # output_file.write(raw_line + '\t' + ' '.join(a1c_results) + '\n')

input_file.close()
output_file.close()