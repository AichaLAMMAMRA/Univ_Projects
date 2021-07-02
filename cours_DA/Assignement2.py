#!/usr/bin/env python
# coding: utf-8

# # Assignment 2
# 
# ## Exercise 2.1 
# 
# Describe a procedure that would appy cross-validation for regression for time-series. Note that in that case the element of time should be respected, in the sense that in a time-series we know only the past observations (not the future) and we want to predict the right next one.
# 
# ## Exercise 2.2
# 
# Using the built-in SVM model of scikit learn, implement a multi-class SVM. The two architectures you can experiment with are:  
# * the <*1 class-vs-all classes*> architecture, where you will need $k$ classifiers as many as the classes, and each one of them will be trained to distinguish the objects of one class against any other class.
# * the <*1 class-vs-1 class*> architecture, where you will need $\frac{1}{2}(k-1)^2$ classifiers, each one of them deciding among two classes only.  
# 
# Develop proper voting mechanisms and classification thresholds based on the opinions of the set of classifiers you produce each time.   
# 
# Make a simple demonstration on Iris dataset.

# # Exercice 2.1
# 
# Before going through cross validation for time series, we need first to explain what is cross validation. The answer to this question is that corss validation is a technique used to see how well will the model fit to unknown data. It's based on deviding the set of data that we have into multiple sets, that we will use either to train the model or to test it. It can also be used for tuning hyperparameters.
# 
# Therefore, we can coclude that using this technique for time series involve taking into account new data generated as time goes. there are two reasons why this can make a problem : 
# 1. Temporal Dependencies
# 2. Arbitrary Choice of Test Set
# 
# A way to fix this problem is to use a forward chaining approach, the algorithme will be something such us : 
fold 1 : training day 1, test day 2
fold 2 : training day 1, 2, test day 3
fold 3 : training day 1, 2, 3, test day 4
fold 4 : training day 1, 2, 3, 4, test day 5
fold 5 : training day 1, 2, 3, 4, 5, test day 6

for k days : 

loop i = 1 to k 
    train on day[: to i-1], test on day[i]
performance = sum of err[i]
# To make a more robost model we can take into account only the last period of time (only last 10 days for example). This way we won't have problem with data size. 

# # Exercice 2.1
# Let's impliment a multiclass SVM :
# For this will use two architectures one vs rest and one vs one. We will use the built-in SVM model of scikit learn. We will calculate the Model accuracy each time. 

# One vs rest

# In[3]:


import numpy as np
from sklearn import svm, datasets
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from sklearn.model_selection import train_test_split
from sklearn.metrics import roc_curve, auc
from sklearn.preprocessing import label_binarize
from sklearn.multiclass import OneVsRestClassifier
from sklearn.multiclass import OneVsOneClassifier
from sklearn.svm import LinearSVC

# import some data to play with
iris = datasets.load_iris()
# Take the first two features. We could avoid this by using a two-dim dataset
X = iris.data[:, :2]
y = iris.target




# shuffle and split training and test sets
X_train, X_test, y_train, y_test =     train_test_split(X, y, stratify=y, test_size=0.7)
    
cmap_light = ListedColormap(['#FFAAAA', '#AAFFAA', '#AAAAFF'])
cmap_bold = ListedColormap(['#FF0000', '#00FF00', '#0000FF'])
# we create an instance of SVM and fit out data. We do not scale our
# data since we want to plot the support vectors
C = 1.0  # SVM regularization parameter


#applying all classifiers to an unseen sample x and predicting the label k for which the corresponding classifier reports the highest confidence score 
clf = OneVsRestClassifier(svm.SVC(kernel='linear', probability=True))
y_score = clf.fit(X_train, y_train).decision_function(X_test)


h = .02  # step size in the mesh

x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                         np.arange(y_min, y_max, h))
Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])

    # Put the result into a color plot
Z = Z.reshape(xx.shape)
plt.figure()
plt.pcolormesh(xx, yy, Z, cmap=cmap_light)

    # Plot also the training points
plt.scatter(X[:, 0], X[:, 1], c=y, cmap=cmap_bold,
                edgecolor='k', s=20)
plt.title(" SVM classification one vs rest")
plt.axis('tight')
plt.axis('tight')

# Calculate model accuracy 
score = clf.score(X_test, y_test)
print("Your Model Accuracy is", score)




# We can plot ROC curve for the second class for example

# In[4]:


# Binarize the output to extend algorithms to the multi-class classification
y = label_binarize(y, classes=[0, 1, 2])
n_classes = y.shape[1]

X_train, X_test, y_train, y_test =     train_test_split(X, y, stratify=y, test_size=0.7)

clf = OneVsRestClassifier(svm.SVC(kernel='linear', probability=True))
y_score = clf.fit(X_train, y_train).decision_function(X_test)

# Compute ROC curve and ROC area and plot
fpr = dict()
tpr = dict()
roc_auc = dict()
for i in range(n_classes):
    fpr[i], tpr[i], _ = roc_curve(y_test[:, i], y_score[:, i])
    roc_auc[i] = auc(fpr[i], tpr[i])

clf.fit(X_train, y_train)
score = clf.score(X_test, y_test)

plt.figure()
lw = 1
plt.plot(fpr[1], tpr[1], color='darkorange',
         lw=lw, label='ROC curve (area = %0.2f)' % roc_auc[1])
plt.plot([0, 1], [0, 1], color='navy', lw=lw, linestyle='--')
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic example')
plt.legend(loc="lower right")
plt.show()


# One vs one

# In[2]:




# one vs one 
X = iris.data[:, :2]
y = iris.target

#This is implemented as ``argmax(decision_function(X), axis=1)`` which will return the label of the class with most votes by estimators predicting the outcome of a decision for each possible class pair.


# shuffle and split training and test sets
X_train, X_test, y_train, y_test =     train_test_split(X, y, stratify=y, test_size=0.7)
    
clf = OneVsOneClassifier(LinearSVC(max_iter=1000000, random_state=0))
y_score = clf.fit(X_train, y_train).decision_function(X_test)


 

h = .02  # step size in the mesh

x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                         np.arange(y_min, y_max, h))
Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])

    # Put the result into a color plot
Z = Z.reshape(xx.shape)
plt.figure()
plt.pcolormesh(xx, yy, Z, cmap=cmap_light)

    # Plot also the training points
plt.scatter(X[:, 0], X[:, 1], c=y, cmap=cmap_bold,
                edgecolor='k', s=20)
plt.title(" SVM linear Class classification one vs one")
plt.axis('tight')
plt.axis('tight')

# Calculate model accuracy 
score = clf.score(X_test, y_test)
print("Your Model Accuracy is", score)


# We can notice that both of architectures gives good results. the one vs one needs more iterations to get this result comparing to OvR. 
