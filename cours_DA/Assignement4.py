#!/usr/bin/env python
# coding: utf-8

# ## Exercise 6
# 
# ### **Incredients:**
#  
# * **$k$-Nearest Neighbors** ($k$NN) classifier. Use the built-in model of scikit learn. This was discussed in ``Section 2 - Nearest Neighbors Classification of Lab 4``.
# 
# * **Accuracy** is a quality index that measures how correct is the classification of new objects and in order to compute it, one needs to use datasets where the class labels for the data objects are known.
# 
# * **NN consistency** is another quality measure for this kind of distance-based classifiers. What it measures is the class agreement, in other words: the expected percentage of the $k$ NNs around any data object to be of the same class as that object. This computation can be done for $k$ in a value range, e.g. $k\in\{1,...,10\}$, and this way describe how the classifier's consistency is affected with $k$. 
# 
# ### **Recipe to prepare:**
# 
# In this exercise you have to simulate data generated from a mixture of 2 Gaussian distributions, and then measure the effect on NN consistency and Accuracy with regards to the following factors:
# 
# * the size of the dataset; try generating classes of same standard deviation $\sigma = 2$ and equal size, where each of them will have $n_i\in\{50, 100, 200, 500\}$ objects;
# * the dimensionality $d$ of the data; try $d\in\{1,2,3,...\,,10, 20, 30,...\,100\}$;
# * the overlap of the classes $\delta$ in the feature space, i.e. the distance between the classes; try $\delta\in\{\sigma, 2\sigma, 3\sigma, 4\sigma\}$;
# * the input parameter $k$ which indirectly adjusts how far from an object the classifier will seek for `advices' from other objects in order to classify it; try $k\in\{1,...\,,10\}$.
# 
# ### **Sample pseudocode:**  
# 
# for each tuple <$n$, $d$, $\delta$, $k$>
#    * generate data  
#    * compute average quality over 10 runs of the following  
#    * train a $k$NN classifier  
#    * test the $k$NN classifier using accuracy  
# 
# end for
# 
# The evaluation should be performed with and without 10-fold CV (inside the train/test nested part). Use at least the suggested value range for each parameter, but you can extend it if this way you capture some interesting behavior. 
# 
# **Question:** What is the main difference between a $k$NN and a radius-NN classifier? In which case(s) one is better to the other? How would we have to adapt the above discussion if the second variant was to be considered instead of the $k$NN?

# ## Solution 

# In this exercise will use sklearn make_classification tool that generate a random n-class classification problem. n-samples is the total size of classes (we can choose the repartition by fixing weight parametre) in our case we will have an equilibrated distribution. n_features is X lenght and n_classes is the number of classes that we genrate. class_sep is the distance between the classes and scale is the standard deviation.

# In[5]:


import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from sklearn import neighbors
from sklearn.datasets import make_classification
from sklearn.model_selection import train_test_split

# 
X, y = make_classification(n_samples=100, n_features=2, n_informative=2, n_redundant=0, n_repeated=0, n_classes=2, n_clusters_per_class=1, weights=None, flip_y=0., class_sep=5.0, hypercube=True, shift=0.0, scale=2.0, shuffle=True, random_state=None)

plt.scatter(X[:, 0], X[:, 1], c=y,edgecolor='k')


# We will also use KNeighborsClassifier where we will change k (number of neighbors that model will ask). we split data to test and train set (30% - 70%). then we will calculate Model accuracy.

# In[2]:


X_train, X_test, y_train, y_test =     train_test_split(X, y, stratify=y, test_size=0.3)
h = .1  # step size in the mesh

# Create color maps
cmap_light = ListedColormap(['#FFAAAA', '#AAFFAA', '#AAAAFF'])
cmap_bold = ListedColormap(['#FF0000', '#00FF00', '#0000FF'])
                            
clf = neighbors.KNeighborsClassifier(5, weights='uniform')
clf.fit(X_train, y_train)

    # Plot the decision boundary. For that, we will assign a color to each
    # point in the mesh [x_min, x_max]x[y_min, y_max].
x_min, x_max = X_test[:, 0].min() - 1, X_test[:, 0].max() + 1
y_min, y_max = X_test[:, 1].min() - 1, X_test[:, 1].max() + 1
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
plt.xlim(xx.min(), xx.max())
plt.ylim(yy.min(), yy.max())
plt.title("3-Class classification (k = %i, weights = '%s')"
              % (5, 'uniform'))

plt.show()

score = clf.score(X_test, y_test)
print("Your Model Accuracy is", score)


# Now we will change all fort parametre and calculate Accuracy. We will try to plot that after. 

# In[3]:


get_ipython().run_line_magic('matplotlib', 'inline')
from ipywidgets import interactive
import matplotlib.pyplot as plt

def f(n, d, delta, k):
    X, y = make_classification(n_samples=n, n_features=d, n_informative=2, n_redundant=0, n_repeated=0, n_classes=2, n_clusters_per_class=1, weights=None, flip_y=0., class_sep=delta, hypercube=True, shift=0.0, scale=2.0, shuffle=True, random_state=None)

    X_train, X_test, y_train, y_test =         train_test_split(X, y, stratify=y, test_size=0.3)
    if d == 2:
   
        h = .1  # step size in the mesh
                    
        # Create color maps
        cmap_light = ListedColormap(['#FFAAAA', '#AAFFAA', '#AAAAFF'])
        cmap_bold = ListedColormap(['#FF0000', '#00FF00', '#0000FF'])
                            
    clf = neighbors.KNeighborsClassifier(k, weights='uniform')
    clf.fit(X_train, y_train)
    #plot only 2 dimensions 
    if d == 2:
        # Plot the decision boundary. For that, we will assign a color to each
        # point in the mesh [x_min, x_max]x[y_min, y_max].
        x_min, x_max = X_test[:, 0].min() - 1, X_test[:, 0].max() + 1
        y_min, y_max = X_test[:, 1].min() - 1, X_test[:, 1].max() + 1
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
        plt.xlim(xx.min(), xx.max())
        plt.ylim(yy.min(), yy.max())
        plt.title("3-Class classification (k = %i, weights = '%s')"
              % (k, 'uniform'))

        plt.show()
    #calculat accuracy for all
    score = clf.score(X_test, y_test)
    print("Your Model Accuracy is", score)
    


# In[4]:


#We need to use notebook widgets to create a slider for each parameter with choosen range
interactive_plot = interactive(f, n=(50, 1000,50), d=(2,100,5), delta=(2,8,2), k=(1,10,1))
output = interactive_plot.children[-1]
output.layout.height = '350px'
interactive_plot


# This widget permet us to change tuple and see how to model react to that. our two classes are very well separated and that is logical knowing that we have choosen 2 gaussian distributions. Although, we can see that accuracy is affacted in case of high amount of data and high dimension and a small delta. in our case more neighbors we take the better it is, till a certain point(the other class star influenting). we can go further by changing k range samples range and take a smaller delta and sigma. at the end we can apreciate KNN robustness and ease. 

# In[ ]:




