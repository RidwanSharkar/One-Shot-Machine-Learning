**One-Shot House Price Estimation Using Machine Learning**

Overview:
This C program implements a simple machine learning algorithm to estimate house prices based on various attributes such as the number of bedrooms, bathrooms, square footage, and year of construction. The program utilizes a form of "one-shot learning", consisting of a linear regression model and matrix operations, that determines weight coefficients that best fit a provided training dataset to predict future values. The learning process involves finding the pseudo-inverse of the matrix formed by the training set, which is then used to solve the normal equations derived from minimizing the sum of squared differences between the predicted and actual prices. These weights are then applied to new data to predict house prices. 

________________________________________________________

Compiling:
gcc -o estimate estimate.c -std=c99 -Wall -Werror -lm

Running:
The program takes two command-line arguments: the path to the training data file and the path to the input data file. Use the following command to run the program:
./estimate train.txt data.txt
Here, train.txt should contain the training data with house attributes and their corresponding prices, while data.txt contains new house data for which prices are to be predicted.
