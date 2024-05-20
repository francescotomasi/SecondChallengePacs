#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP
// clang-format off
#include <array>
#include <cstddef>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include <complex>
#include <fstream>

namespace algebra {
    
    // Enumerator for specifying the storage order
    enum class StorageOrder { RowMajor, ColumnMajor };

    enum class NormType { One, Infinity, Frobenius};
  
    // Dynamic matrix class template
    template <class T, StorageOrder Order>
    class SparseMatrix {
   
    private:
    //@note the simplest way to account for row and column major is to change the comparison operator in the map
        std::map<std::array<std::size_t, 2>, T> m_data;
        size_t rows; //Number of rows
        size_t cols; //Number of colums
        bool compression;
        std::vector<T> values; // Values of non-zero elements
        std::vector<size_t> indices; // Indices of non-zero elements
        std::vector<size_t> ptr; // Pointers to start of each row/column
   
    public:
        /// Default constructor.
        SparseMatrix(): rows(0), cols(0), compression(false) {}

        // Constructor
        SparseMatrix(size_t rows, size_t cols): rows(rows), cols(cols), compression(false) {};

        // Getter
        size_t get_cols() const;

       // Method to compress the matrix
        void compress();
           
        // Method to uncompress the matrix
        void uncompress();

        // Method to check if the matrix is compressed
        bool is_compressed() const;

        // Method to resize
        void resize(size_t r, size_t c);

        // Method to print matrix
        void print_matrix();

        // Method to extract an element
        T operator() (std::size_t row, std::size_t col) const;
        
        // Method to change the element
        T& operator() (std::size_t row, std::size_t col);

        // moltiplication sparse_matrix*vector 
        template<typename U>
        std::vector<U> operator*(const std::vector<U>& vec) const;

        // read matrix market
        //@note Why static? It can be a member function that changes the state of the class.
        // Or, if you prefer, an external function that takes a reference to the object , if you want the reader not to be a member of the class.
        static SparseMatrix from_matrix_market(const std::string& filename);

    };

} // namespace algebra

#include "sparse_matrix_impl.hpp"
 
#endif /* SPARSE_MATRIX_HPP */