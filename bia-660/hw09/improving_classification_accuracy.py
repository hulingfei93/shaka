# -*- coding: utf-8 -*-
"""
Created on Thu Oct 29 19:53:45 2015

@author: Lingfei Hu
"""

"""
A simple script that demonstrates how we classify textual data with sklearn.
"""
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.metrics import accuracy_score
from sklearn.naive_bayes import GaussianNB


#read the reviews and their polarities from a given file
def loadTrainData(fname):
    reviews=[]
    labels=[]
    f=open(fname)
    for line in f:
        review,rating=line.strip().split('\t')  
        reviews.append(review.lower())    
        labels.append(int(rating))
    f.close()
    return reviews,labels
    
def loadTestData(fname):
    reviews=[]
    f=open(fname)
    for line in f:
        review=line.strip()
        reviews.append(review.lower())    
    f.close()
    return reviews

rev_train,labels_train=loadTrainData('train.txt')
#rev_test,labels_test=loadTrainData('reviews_test.txt')
rev_test=loadTestData('test.txt')

#Build a counter based on the training dataset
counter = CountVectorizer()
counter.fit(rev_train)

#count the number of times each term appears in a document and transform each doc into a count vector
counts_train = counter.transform(rev_train)#transform the training data
counts_test = counter.transform(rev_test)#transform the testing data

clf = GaussianNB()
clf.fit(counts_train.todense(),labels_train)
predicted = clf.predict(counts_test.todense())

output_file = open("predictions.txt", "w")
for score in predicted:
    output_file.write(str(score) + '\n')
output_file.close()

#print accuracy_score(predicted,labels_test)



