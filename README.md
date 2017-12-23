# BayesianClassification
Implementation of Bayesian Classification

Code works on '20 Newsgroups' dataset and is able to achive 87% accuracy with correct blacklist.

First the application must be trained by executing
```
Bayesian.exe -L "dataset path"
```
After training completes `<classname>.class` files are generated for all the classes. These files contain histogram for the class.

After training completes document can be classified with
```
Bayesian.exe -t "dataset path"
```
