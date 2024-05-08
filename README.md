# SecondChallengePacs

## Introduction

This project involves the implementation of a sparse matrix representation and performing various operations on it. The primary tasks include:

- Implementing matrix-vector multiplication (Av)
- Reading matrices from the Matrix Market format
- Testing the performance of the code under different scenarios

## Content
**-sparse_matrix.hpp:** This file contains the declaration of the SparseMatrix class template along with its member functions and necessary includes. It provides a framework for representing and manipulating sparse matrices efficiently.

**-sparse_matrix_impl.hpp:** This file contains the implementation of the member functions of the SparseMatrix class template declared in sparse_matrix.hpp. It includes the actual logic for compressing, uncompressing, resizing, printing, and performing matrix-vector multiplication operations.

**main.cpp:** This file contains the main function where the SparseMatrix class is tested. It includes code to read matrices from the Matrix Market format, perform matrix-vector multiplication, and test the performance of the code under different scenarios.

**Makefile:** This file contains the instructions for building the project using the make utility. It specifies the compilation flags, dependencies, and rules for compiling the source files and generating the executable.

## Prerequisites

The Makefile assumes the `g++` compiler is available. If you're using a different compiler or if `g++` is not installed, you would need to change the `CXX` variable in the Makefile to match your compiler.

## Building the Project

To build the project, navigate to the repository in your terminal and execute the following commands:

```bash
make
```

## Running the Project

After building the project, you can run it using the following command:

```bash
./main
```

## Downloading the Repository

To clone this repository to your local machine, run the following command in your terminal:

```bash
git clone https://github.com/yourusername/SecondChallengePacs.git
```

Please replace `yourusername` with your actual GitHub username.
```

Remember to replace `yourusername` with your actual GitHub username. 
