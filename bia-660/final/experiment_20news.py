# -*- coding: utf-8 -*-
"""
Created on Fri Nov 06 15:27:32 2015

@author: Lingfei Hu
"""

from sklearn.datasets import fetch_20newsgroups
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.naive_bayes import MultinomialNB
from sklearn.linear_model import SGDClassifier
from sklearn.pipeline import Pipeline
from sklearn.grid_search import GridSearchCV
import numpy as np
from sklearn import metrics

if __name__ == '__main__':
    categories = ['alt.atheism', 'soc.religion.christian', 'comp.graphics', 'sci.med']
    twenty_train = fetch_20newsgroups(data_home = './', subset='train', categories=categories, shuffle=True, random_state=42)
    twenty_test = fetch_20newsgroups(data_home = './', subset='test', categories=categories, shuffle=True, random_state=42)
    
    # MNB
    print "--------------------MNB----------------------------"
    MNB_pipeline = Pipeline([('vect', CountVectorizer()), ('tfidf', TfidfTransformer()), ('clf', MultinomialNB()),])
    MNB = MNB_pipeline.fit(twenty_train.data, twenty_train.target)
    predicted = MNB.predict(twenty_test.data)
    print np.mean(predicted == twenty_test.target)
    print(metrics.classification_report(twenty_test.target, predicted, target_names=twenty_test.target_names))
    
    # SVM
    print "--------------------SVM----------------------------"
    SVM_pipeline = Pipeline([('vect', CountVectorizer()), 
                             ('tfidf', TfidfTransformer()), 
                             ('clf', SGDClassifier(loss='hinge', penalty='l2', n_iter=5, random_state=42)),
                            ])
    SVM = SVM_pipeline.fit(twenty_train.data, twenty_train.target)
    predicted = SVM.predict(twenty_test.data)
    print np.mean(predicted == twenty_test.target)
    print(metrics.classification_report(twenty_test.target, predicted, target_names=twenty_test.target_names))
    
    SVM_parameters = {'vect__ngram_range': [(1, 1), (1, 2)],
                      'tfidf__use_idf': (True, False),
                      'clf__alpha': (1e-2, 1e-3),
                     }
    SVM_gs = GridSearchCV(cv = 5, estimator = SVM_pipeline, param_grid = SVM_parameters, n_jobs = -1)
    SVM_optimal = SVM_gs.fit(twenty_train.data, twenty_train.target)
    
    best_parameters, score, _ = max(SVM_gs.grid_scores_, key=lambda x: x[1])
    for param_name in sorted(SVM_parameters.keys()):
        print("%s: %r" % (param_name, best_parameters[param_name]))
    print "score: " + str(score)
    
    predicted = SVM_optimal.predict(twenty_test.data)
    print np.mean(predicted == twenty_test.target)
    print(metrics.classification_report(twenty_test.target, predicted, target_names=twenty_test.target_names))

