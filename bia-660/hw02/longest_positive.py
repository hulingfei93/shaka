# -*- coding: utf-8 -*-
"""
Created on Thu Sep 17 19:48:08 2015

@author: Lingfei Hu
"""

def loadLexicon(fname):
    new_lex = set()
    lex_conn = open(fname)
    
    for line in lex_conn:
        new_lex.add(line.strip())
        
    lex_conn.close()
    
    return new_lex
    

positive_lex = loadLexicon("positive-words.txt")

input_file = open('input.txt')
longest_line = ''
longest_count = 0

for line in input_file:
    line = line.strip()
    words = line.split(' ')
    
    words_count = len(words)
    if words_count >= 42:
        continue
    
    if words_count <= longest_count:
        continue
    
    longest_count = words_count
    longest_line = line
    
input_file.close()


# write results
result_file = open('long_positive.txt', 'w')

for word in longest_line.split(' '):
    if word in positive_lex:
        result_file.write(word + '\n')
    
result_file.close()