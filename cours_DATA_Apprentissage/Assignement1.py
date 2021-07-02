#!/usr/bin/env python
# coding: utf-8

# # Assignment 1 REPORT
# 
# In the following the answer for the first assignment 
# 
# ## Exercise 1.1 
# 
# we need to change the first 10 data points by adding 50 to the their coordinate values. to do that we need to make a small change of the script as it shown.
# 

# In[1]:


from sklearn import linear_model
reg = linear_model.LinearRegression()
reg.fit([[0, 0], [1, 1], [2, 2]], [0, 1, 2])
                                      
reg.coef_


# In[2]:


# Import modules and libraries
import matplotlib.pyplot as plt
import numpy as np
from sklearn import datasets, linear_model
from sklearn.metrics import mean_squared_error, r2_score


# In[3]:


# Load the diabetes dataset (should come with your scikit installation)
diabetes = datasets.load_diabetes()

# Optionally inspect the dataset as python stored it in data structures
diabetes


# In[4]:


# Use only one feature
X = diabetes.data[:, np.newaxis, 2]

# Split the data into training/testing sets
X_train = X[:-20]
X_test = X[-20:]

# Split the targets into training/testing sets
y_train = diabetes.target[:-20]

y_test = diabetes.target[-20:]


# In[5]:


# Create a "linear regression" object
regr = linear_model.LinearRegression()
# Train the model using the training sets
regr.fit(X_train, y_train)

# Make predictions using the testing set
y_pred = regr.predict(X_test)


# In[6]:


# Show results 

# The learned coefficients
print('Coefficients: \n', regr.coef_)

# The mean squared error
print("Mean squared error: %.2f" 
      % mean_squared_error(y_test, y_pred))
# Explained variance score: 1 is perfect prediction
print('Variance score: %.2f' % r2_score(y_test, y_pred))

# Plot the fit on the training data
plt.figure(1)
plt.scatter(X_train, y_train,  color='blue')
plt.plot(X_test, y_pred, color='blue', linewidth=3)

# Plot outputs
plt.figure(2)
plt.scatter(X_test, y_test,  color='black')
plt.plot(X_test, y_pred, color='blue', linewidth=3)

plt.show()


# In[7]:


count = 0
while count < 10:
    y_train[count] = y_train[count] + 50
    count += 1
print(y_train) # We can see that we had added 50 to the first 10 values 

regr = linear_model.LinearRegression()
regr.fit(X_train, y_train)
y_pred = regr.predict(X_test)
# Plot the fit on the training data
plt.figure(1)
plt.scatter(X_train, y_train,  color='blue')
plt.plot(X_test, y_pred, color='blue', linewidth=3)

# Plot outputs
plt.figure(2)
plt.scatter(X_test, y_test,  color='black')
plt.plot(X_test, y_pred, color='blue', linewidth=3)

plt.show()


# In[8]:


# Now let's do this multiple time 
new_count = 1
while new_count < 20:
    count = 0
    while count < 10:      #changing the number of points by increasing the boundry <20 or <50
        y_train[count] = y_train[count] + 50
        count += 1
    
    regr = linear_model.LinearRegression()
    regr.fit(X_train, y_train)
    y_pred = regr.predict(X_test)

# Plot the fit on the training data
    plt.figure(1)
    plt.scatter(X_train, y_train,  color='blue')
    plt.plot(X_test, y_pred, color='blue', linewidth=3)

# Plot outputs
    plt.figure(2)
    plt.scatter(X_test, y_test,  color='black')
    plt.plot(X_test, y_pred, color='blue', linewidth=3)

    plt.show()
    new_count += 1


# # Comment
# We can notice that every time the ten data points get higher, the continuum follows. if we play with it and add more points will notice a further and faster change 
# We can also see that this changes also affect the result for the test data.
# In a real situation, we can observe that outliers can leed us to wrong results. A method to fix this problem is explained in the second part of the exercice. 

# ## Exercice 1.2 : 

# The algorithme for the Random sample consensus is as follow 
Given:
    data – a set of observations
    model – a model to explain observed data points
    n – minimum number of data points required to estimate model parameters
    k – maximum number of iterations allowed in the algorithm
    t – threshold value to determine data points that are fit well by model 
    d – number of close data points required to assert that a model fits well to data

Return:
    bestFit – model parameters which best fit the data (or nul if no good model is found)

iterations = 0
bestFit = nul
bestErr = something really large
while iterations < k {
    maybeInliers = n randomly selected values from data
    maybeModel = model parameters fitted to maybeInliers
    alsoInliers = empty set
    for every point in data not in maybeInliers {
        if point fits maybeModel with an error smaller than t
             add point to alsoInliers
    }
    if the number of elements in alsoInliers is > d {
        % this implies that we may have found a good model
        % now test how good it is
        betterModel = model parameters fitted to all points in maybeInliers and alsoInliers
        thisErr = a measure of how well betterModel fits these points
        if thisErr < bestErr {
            bestFit = betterModel
            bestErr = thisErr
        }
    }
    increment iterations
}
return 

# # The implementation:
# 

# In[9]:


import numpy as np
from matplotlib import pyplot as plt
from sklearn import linear_model, datasets
np.warnings.filterwarnings('ignore')



n_samples = 1000
n_outliers = 50

# Generate a random regression problem of the desired characteristics
X, y, coef = datasets.make_regression(n_samples=n_samples, n_features=1,
                                      n_informative=1, noise=10,
                                      coef=True, random_state=0)

# Add outlier data
np.random.seed(0)
X[:n_outliers] = 3 + 0.5 * np.random.normal(size=(n_outliers, 1))
y[:n_outliers] = -3 + 10 * np.random.normal(size=n_outliers)


# In[10]:


def Myransac(data,model,n,k,t,d,return_all=False):

    iterations = 0
    bestfit = None
    besterr = np.inf
    best_inlier_idxs = None
    while iterations < k:
        maybe_idxs, test_idxs = random_partition(n,data.shape[0])
        maybeinliers = data[maybe_idxs,:]
        test_points = data[test_idxs]
        maybemodel = model.fit(maybeinliers)
        test_err = model.get_error( test_points, maybemodel)
        also_idxs = test_idxs[test_err < t] 
        alsoinliers = data[also_idxs,:]
        if len(alsoinliers) > d:
            betterdata = np.concatenate( (maybeinliers, alsoinliers) )
            bettermodel = model.fit(betterdata)
            better_errs = model.get_error( betterdata, bettermodel)
            thiserr = np.mean( better_errs )
            if thiserr < besterr:
                bestfit = bettermodel
                besterr = thiserr
                best_inlier_idxs = np.concatenate( (maybe_idxs, also_idxs) )
        iterations+=1
    if bestfit is None:
        raise ValueError("did not meet fit acceptance criteria")
    if return_all:
        return bestfit, {'inliers':best_inlier_idxs}
    else:
        return bestfit
    
# We need to define a function to get us a random selection of the data

def random_partition(n,n_data):
    """return n random rows of data (and also the other len(data)-n rows)"""
    all_idxs = np.arange( n_data )
    np.random.shuffle(all_idxs)
    idxs1 = all_idxs[:n]
    idxs2 = all_idxs[n:]
    return idxs1, idxs2

# we will use least squares model

class LinearLeastSquaresModel:

    def __init__(self,input_columns,output_columns,debug=False):
        self.input_columns = input_columns
        self.output_columns = output_columns
        self.debug = debug
    def fit(self, data):
        A = np.vstack([data[:,i] for i in self.input_columns]).T
        B = np.vstack([data[:,i] for i in self.output_columns]).T
        x,resids,rank,s = np.linalg.lstsq(A,B)
        return x
    def get_error( self, data, model):
        A = np.vstack([data[:,i] for i in self.input_columns]).T
        B = np.vstack([data[:,i] for i in self.output_columns]).T
        B_fit = np.dot(A,model)
        err_per_point = np.sum((B-B_fit)**2,axis=1) # sum squared error per row
        return err_per_point


# In[13]:


# let's use our function and see the results 

n_samples = 1000
n_inputs = 1
n_outputs = 1
y = y.reshape(1000,1)

n_outliers = 50
all_idxs = np.arange( X.shape[0] )
np.random.shuffle(all_idxs)
outlier_idxs = all_idxs[:n_outliers]
non_outlier_idxs = all_idxs[n_outliers:]

# setup model

all_data = np.hstack( (X,y) )
input_columns = range(n_inputs) # the first columns of the array
output_columns = [n_inputs+i for i in range(n_outputs)] # the last columns of the array
debug = False
model = LinearLeastSquaresModel(input_columns,output_columns,debug=debug) # We can notice how this program is generic by choosing any model we want  

linear_fit,resids,rank,s = np.linalg.lstsq(all_data[:,input_columns],
                                              all_data[:,output_columns])

# run RANSAC algorithm
ransac_fit, ransac_data = Myransac(all_data,model,
                                 50, n_samples, 7e3, 300,
                                 return_all=True)

# plot results
import pylab

sort_idxs = np.argsort(X[:,0])
X_col0_sorted = X[sort_idxs] # maintain as rank-2 array

if 1:
    pylab.plot( X[:,0], y[:,0], 'k.', label='data' )
    pylab.plot( X[ransac_data['inliers'],0], y[ransac_data['inliers'],0], 'bx', label='RANSAC data' )
else:
        pylab.plot( X[non_outlier_idxs,0], y[non_outlier_idxs,0], 'k.', label='noisy data' )
        pylab.plot( X[outlier_idxs,0], y[outlier_idxs,0], 'r.', label='outlier data' )
pylab.plot( X_col0_sorted[:,0],
           np.dot(X_col0_sorted,ransac_fit)[:,0],
           label='RANSAC fit' )
pylab.plot( X_col0_sorted[:,0],
           np.dot(X_col0_sorted,linear_fit)[:,0],
           label='linear fit' )
pylab.legend(loc='lower right')
pylab.show()


# Comment 
# we can clearly see how the ransac algorithme take out all outlires from data considered by the model. This gives us a better fitting. 

# Now let's see np RANSACRegressor() 

# In[12]:



# Fit line using all data
lr = linear_model.LinearRegression()
lr.fit(X, y)

# Robustly fit linear model with RANSAC algorithm
ransac = linear_model.RANSACRegressor()
ransac.fit(X, y)
inlier_mask = ransac.inlier_mask_
outlier_mask = np.logical_not(inlier_mask)

# Predict data of estimated models
line_X = np.arange(X.min(), X.max())[:, np.newaxis]
line_y = lr.predict(line_X)
line_y_ransac = ransac.predict(line_X)

# Compare estimated coefficients
print("Estimated coefficients (true, linear regression, RANSAC):")
print(coef, lr.coef_, ransac.estimator_.coef_)

lw = 2
plt.scatter(X[inlier_mask], y[inlier_mask], color='yellowgreen', marker='.',
            label='Inliers')
plt.scatter(X[outlier_mask], y[outlier_mask], color='gold', marker='.',
            label='Outliers')
plt.plot(line_X, line_y, color='navy', linewidth=lw, label='Linear regressor')
plt.plot(line_X, line_y_ransac, color='cornflowerblue', linewidth=lw,
         label='RANSAC regressor')
plt.legend(loc='lower right')
plt.xlabel("Input")
plt.ylabel("Response")
plt.show()


# We can notice that we have same results as before 
