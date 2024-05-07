# Clustering Algorithms Project:
** Symmetric Non-negative Matrix Factorization (SymNMF) **
** Kmeans Algorithm **

## Overview
This project implements a clustering algorithm based on Symmetric Non-negative Matrix Factorization (SymNMF), comparing its effectiveness to the K-means algorithm. It involves the creation of several components in both Python and C to achieve the desired functionality.

## Components
### Python Files
- **symnmf.py**: Main Python script for executing the SymNMF algorithm.
- **kmeans.py**: Main Python script for executing the kmeans algorithm.
- **analysis.py**: Script to analyze and compare the clustering results using SymNMF and K-means.

### C Files
- **symnmf.c**: Core C implementation of the SymNMF algorithm.
- **symnmf.h**: Header file for the C implementation.
- **symnmfmodule.c**: Python C API wrapper for integrating the C code with Python.

### Build Files
- **setup.py**: Setup script to build Python extensions.
- **Makefile**: Script to build the C implementation.

## Functionality

### Clustering Algorithm
- Formulate a similarity matrix based on a dataset of points.
- Compute the diagonal degree matrix and a normalized similarity matrix.
- Implement an iterative process to optimize matrix H for the best clustering solution.

### Analysis
- The analysis involves calculating the silhouette score to evaluate clustering performance.
- Comparison between SymNMF and K-means based on silhouette scores.

## Usage

### Building and Running
1. To build the C program:
   ```bash
   make
To run the Python scripts:
bash
Copy code
python3 symnmf.py <args>
python3 analysis.py <args>
Arguments
Python and C programs accept command-line arguments for specifying the number of clusters and input file among other options.

Error Handling
In case of errors, the system will output "An Error Has Occurred" and terminate.

Convergence
For both K-means and NMF methods, convergence is determined by a specified threshold and maximum iteration count.

Examples
Available inside inputs directory

Reference
Da Kuang, Chris Ding, and Haesun Park. "Symmetric nonnegative matrix factorization for graph clustering", 2012.
