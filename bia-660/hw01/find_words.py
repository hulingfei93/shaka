# -*- coding: utf-8 -*-
"""
Created on Thu Sep 10 20:07:18 2015

@author: Lingfei Hu
"""

word_freq=dict()

file_conn=open('input.txt')

for line in file_conn:

    line=line.strip()
    
    if line == '':
        continue

    columns=line.split('@')
  
    state=columns[0]
    name=columns[1]   
    day_of_week=columns[2]
    post = columns[3]

    words=post.split(' ')
    for word in words:
        if word != ' ':
            if word in word_freq:
                word_freq[word]=word_freq[word]+1
            else:
                word_freq[word]=1
    

file_conn.close()

newfile_conn=open('output.txt','w')
for word in word_freq:
    if word_freq[word] == 1:
        newfile_conn.write(word+'\n') 
newfile_conn.close()