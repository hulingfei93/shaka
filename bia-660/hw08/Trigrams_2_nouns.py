# -*- coding: utf-8 -*-
"""
Created on Thu Oct 22 19:45:47 2015

@author: Lingfei Hu
"""

import nltk.data
from nltk.util import ngrams
import re
from nltk.corpus import stopwords

#read the input
f = open('input.txt')
text = f.read().strip().lower()
f.close()

stop_words = set(stopwords.words('english'))

#split sentences
sentences = re.split('\.',text)
print 'NUMBER OF SENTENCES: '+ str(len(sentences))

output_file = open('trigrams.txt', 'w')
 
# for each sentence
for sentence in sentences:

    # nouns dict
    nouns = set()
    
    sentence = re.sub('[^a-z\d]',' ',sentence) #replace chars that are not letters or numbers with a space
    sentence = re.sub(' +',' ',sentence).strip() #remove duplicate spaces

    #tokenize the sentence
    terms = sentence.split()
    #print terms
    
    #do POS tagging on the tokenized sentence
    tagged_terms = nltk.pos_tag(terms)
    
    for pair in tagged_terms: 
        #if the word is an noun
        if pair[1].startswith('NN'):
            nouns.add(pair[0])
        
    #compute 3-grams
    threegrams = ngrams(terms,3) 
    
    #for each 3gram
    for tg in threegrams:
        noun_count = 0
        
        if tg[0] in stop_words or tg[1] in stop_words or tg[2] in stop_words:
            continue
        
        if tg[0] in nouns:
            noun_count = noun_count + 1
        if tg[1] in nouns:
            noun_count = noun_count + 1
        if tg[2] in nouns:
            noun_count = noun_count + 1
        
        if noun_count >= 2:
            output_file.write(tg[0] + ' ' + tg[1] + ' ' + tg[2] + '\n')
            
output_file.close()