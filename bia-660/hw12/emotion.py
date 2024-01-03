# -*- coding: utf-8 -*-
"""
The script reports the top-10 most frequently related nouns per emotion. 
There are 8 different lists for 8 emotios, with a total of 80 words ( 8 x 10 ). 

"""
import re
from nltk.tag import pos_tag
import operator

def writeToFile(dictionary, fileName):
    fileWriter = open(fileName, 'w')
    count = 0
    for key, value in dictionary:
        count = count + 1
        if count == 11: break
        string = key + '\t|\t' + str(value) + '\n'
        fileWriter.write(string)
        fileWriter.write("\n")
    fileWriter.close()

emotionFile=open('emotions.txt')

Anger = []
Anticipation = []
Disgust = []
Fear = []
Joy = []
Sadness = []
Surprise = []
Trust = []
count = 0

for line in emotionFile:
    count = count + 1
    if count > 1:
        wd,anger,anticipation,disgust,fear,joy,sadness,surprise,trust=line.strip().split('\t')  
        if (int(anger) == 1):
            Anger.append(wd)
        if (int(anticipation) == 1):
            Anticipation.append(wd)
        if (int(disgust) == 1):
            Disgust.append(wd)
        if (int(fear) == 1):
            Fear.append(wd)
        if (int(joy) == 1):
            Joy.append(wd)
        if (int(sadness) == 1):
            Sadness.append(wd)
        if (int(surprise) == 1):
            Surprise.append(wd)
        if (int(trust) == 1):
            Trust.append(wd)

dataFile = open('data.txt')

angerNoun = dict()
anticipationNoun = dict()
disgustNoun = dict()
fearNoun = dict()
joyNoun = dict()
sadnessNoun = dict()
surpriseNoun = dict()
trustNoun = dict()

#file cleaning
for line in dataFile:
    
    line = line.strip().lower()
    line = re.sub('[^a-z\d]',' ',line) #Replace chars that are not letters or numbers with a space.
    line = re.sub(' +',' ',line) #remove duplicate spaces
    nouns = set()

    sentence = line.split()
    
    tagged_terms = pos_tag(sentence)# Do POS tagging on the tokenized sentence.
    #print tagged_terms
    propernouns = [word for word,pos in tagged_terms if pos == 'NN']
    #print propernouns
    nouns |= set(propernouns)
    words = line.split(' ')

    for wd in words:
        if wd in Anger:
            for noun in nouns:
                if noun in angerNoun:
                    angerNoun[noun] += 1
                else:
                    angerNoun[noun] = 1
        if wd in Anticipation:
            for noun in nouns:
                if noun in anticipationNoun:
                    anticipationNoun[noun] += 1
                else:
                    anticipationNoun[noun] = 1
        if wd in Disgust:
            for noun in nouns:
                if noun in disgustNoun:
                    disgustNoun[noun] += 1
                else:
                    disgustNoun[noun] = 1
        if wd in Fear:
            for noun in nouns:
                if noun in fearNoun:
                    fearNoun[noun] += 1
                else:
                    fearNoun[noun] = 1
        if wd in Joy:
            for noun in nouns:
                if noun in joyNoun:
                    joyNoun[noun] += 1
                else:
                    joyNoun[noun] = 1
        if wd in Sadness:
            for noun in nouns:
                if noun in sadnessNoun:
                    sadnessNoun[noun] += 1
                else:
                    sadnessNoun[noun] = 1
        if wd in Surprise:
            for noun in nouns:
                if noun in surpriseNoun:
                    surpriseNoun[noun] += 1
                else:
                    surpriseNoun[noun] = 1
        if wd in Trust:
            for noun in nouns:
                if noun in trustNoun:
                    trustNoun[noun] += 1
                else:
                    trustNoun[noun] = 1
        
is_Anger = sorted(angerNoun.items(), key=operator.itemgetter(1), reverse=True)
is_Anticipation = sorted(anticipationNoun.items(), key=operator.itemgetter(1), reverse=True)
is_Disgust = sorted(disgustNoun.items(), key=operator.itemgetter(1), reverse=True)
is_Fear = sorted(fearNoun.items(), key=operator.itemgetter(1), reverse=True)
is_Joy = sorted(joyNoun.items(), key=operator.itemgetter(1), reverse=True)
is_Sadness = sorted(sadnessNoun.items(), key=operator.itemgetter(1), reverse=True)
is_Surprise = sorted(surpriseNoun.items(), key=operator.itemgetter(1), reverse=True)
is_Trust = sorted(trustNoun.items(), key=operator.itemgetter(1), reverse=True)


writeToFile(is_Anger, 'Anger.txt') 
writeToFile(is_Anticipation, 'Anticipation.txt')
writeToFile(is_Disgust, 'Disgust.txt')
writeToFile(is_Fear, 'Fear.txt')
writeToFile(is_Joy, 'Joy.txt')
writeToFile(is_Sadness, 'Sadness.txt')
writeToFile(is_Surprise, 'Surprise.txt')
writeToFile(is_Trust, 'Trust.txt')

emotionFile.close()