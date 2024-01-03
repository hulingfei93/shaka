# -*- coding: utf-8 -*-
"""
Created on Thu Nov 05 19:40:33 2015

@author: Lingfei Hu
"""

import nltk.data
import re

_POS_TAGGER = 'taggers/maxent_treebank_pos_tagger/english.pickle' 
tagger=nltk.data.load( _POS_TAGGER ) 

#read the input
input_file = open('input.txt')
output_file = open('negations.txt', 'w')

for sentence in input_file:
    
    results = []
    
    #remove duplicate spaces
    sentence = re.sub(' +',' ',sentence).strip()
    
    tokens = sentence.split()
    tokens_lower = sentence.lower().split()
    tagged_tokens = tagger.tag(tokens_lower)
    token_len = len(tokens)
    
    for i, tagged_token in enumerate(tagged_tokens):
        
        if tagged_token[0] != 'not':
            continue
        
        if i + 1 < token_len:
            candidate =  tagged_tokens[i + 1]
            if candidate[1].startswith('JJ'):
                results.append(tokens[i + 1])
                continue
                
        if i + 2 < token_len:
            candidate =  tagged_tokens[i + 2]
            if candidate[1].startswith('JJ'):
                results.append(tokens[i + 2])
                
        
        
    for i, adj in enumerate(results):
        if i == 0:
            output_file.write(adj)
        else:
            output_file.write(',' + adj)


    output_file.write('\n')


#close files
input_file.close()
output_file.close()