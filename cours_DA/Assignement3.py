#!/usr/bin/env python
# coding: utf-8

# ## Exercise 5 
# 
# Describe and develop an SVM Ensemble for classification using the *bagging* architecture, that is an Ensemble model that is composed out of many weak SVM classifiers. 
# 
# * Demonstrate its performance and behavior in a benchmark dataset like the Iris dataset or any other dataset used in the exercises (use about 20 SVMs, or justify a different choice empirically), also compare that performance against the non-Ensemble learning with the same type of base model... (this is multi-class SVM). 
# 
# * At first do the analysis and experiments for the first two classes of the dataset, and then extend to all the classes.
# 
# * What is the difference (architecture, principle of learining) between an SVM Ensemble and the multi-class SVM architectures developed in Exercise 4? 

# ## Let's see how it works
# 
# Ensemble methods are based on the idea that: multiple weak models (with not particularly good, but just *sufficient* performance) can be used to synthesize a decision our of their individual decisions, and achieve better performance than a single very well-performing model on its own.
# 
# 
# We can say in a way that using this technique will give us the following results :
# 20 SVM with 20% random sampling >>> 1 SVM with 90% sampling and 10% test. ( we will aviod over fiting ) 
# since we are using bagging, we will use 20 random samples with replacement ( one data can figure in several samples )
# 
# 
# * The difference (architecture, principle of learining) between an SVM Ensemble and the multi-class SVM architectures developed in Exercise 4?
#     Multi-class SVM : a method to classify data into multiple class
#     SVM Ensemble : a technique to optimize the learning and aviod over fiting 
#     
#     Muulti-class SVM : finding the best seprating hyperplan using all data at once 
#     SVM Ensemble : we ask all hyperplans ( generated with sample 1, 2, 3... ) and we take the majority vote
#     
# 

# ## Let's try it on a couple of classes 
# 

# In[1]:


from sklearn.svm import LinearSVC
from sklearn.ensemble import BaggingClassifier
import numpy as np
from sklearn import datasets
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from sklearn.model_selection import train_test_split
from sklearn import model_selection

iris = datasets.load_iris()
X, y = iris.data[:, 1:3], iris.target
# Spliting data to train set and test set. This may be changed to see how model reacts 
X_train, X_test, y_train, y_test =     train_test_split(X, y, stratify=y, test_size=0.8)
 
cmap_light = ListedColormap(['#FFAAAA', '#AAFFAA', '#AAAAFF'])
cmap_bold = ListedColormap(['#FF0000', '#00FF00', '#0000FF'])
# Define model, we will use built in SVC and Bagging classifier to get an SVM Ensemble method
svm = LinearSVC(max_iter=10000, random_state=42)
model = BaggingClassifier(base_estimator=svm, n_estimators=20, random_state=314)

# We now use the usual SVC
clf = LinearSVC(max_iter=10000, random_state=42)
clf.fit(X_train, y_train)
score2 = clf.score(X_test, y_test)
# Fit
model.fit(X_train, y_train)
# Calculating score for SVM Ensemble 
score = model_selection.cross_val_score(model, X_test, y_test)

print("Your SVM Ensemble Model Accuracy is", score.mean())
print("Your SVM multi-class Model Accuracy is", score2)



# In[2]:


h = .02  # step size in the mesh

x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                         np.arange(y_min, y_max, h))
Z = model.predict(np.c_[xx.ravel(), yy.ravel()])

    # Put the result into a color plot
Z = Z.reshape(xx.shape)
plt.figure()
plt.pcolormesh(xx, yy, Z, cmap=cmap_light)

    # Plot also the training points
plt.scatter(X[:, 0], X[:, 1], c=y, cmap=cmap_bold,
                edgecolor='k', s=20)
plt.title(" SVM Ensemble")
plt.axis('tight')
plt.axis('tight')


# # Comment
# 
# We can see through this simple application that we get a slitly diffrence results in term of model accuracy. at the end even if we splitted the data to three classes both of model reacts good and give good classification. SVM Ensemble is a little bit better. we can change sampling percentage and see how accuracy changes but most of the times SVM Ensemble will still give better resulats. We delebritly choosen a big set of test to see weakness of the previous model.  
# 

# # Let's extend to all Classes

# In[3]:


X, y = iris.data, iris.target
X_train, X_test, y_train, y_test =     train_test_split(X, y, stratify=y, test_size=0.2)
 
# Define model 
svm = LinearSVC(max_iter=10000, random_state=42)
model = BaggingClassifier(base_estimator=svm, n_estimators=20, random_state=314)

clf = LinearSVC(max_iter=10000, random_state=42)
clf.fit(X_train, y_train)
score2 = clf.score(X_test, y_test)
# Fit
model.fit(X_train, y_train)
score = model_selection.cross_val_score(model, X_test, y_test)
print("Your SVM Ensemble Model Accuracy is", score.mean())
print("Your SVM multi-class Model Accuracy is", score2)


# Once again we can notice that both of methods gives satisfying results. The best model depends on how much data we train the model on. results in term of accuracy are very close.
