#ifndef SPARSE_MATRIX_IMPL_HPP
#define SPARSE_MATRIX_IMPL_HPP

#include "sparse_matrix.hpp"
// clang-format off
namespace algebra {

template <class T, StorageOrder Order>
size_t SparseMatrix<T, Order>::get_cols() const {
  return cols;
}

template <class T, StorageOrder Order> void SparseMatrix<T, Order>::compress() {
  //@note you should check if already compressed and in that case just return.
  // Clear compressed data vectors
  values.clear();
  indices.clear();
  ptr.clear();
  //@note Remember that clear() un vectors set size to zero but not the
  //capacity. If you want to
  // free memory you have to use shrink_to_fit() method after clear().

  //@note since you know the number of rows and columns you can reserve the
  //space in the vectors
  // and increase afficiency and memory usage.
  // Convert map to CSR/CSC format
  ptr.push_back(0); // Start pointer

  if constexpr (Order == StorageOrder::RowMajor) {
    // Compress to CSR format
    size_t current_row = 0;
    for (auto it = m_data.begin(); it != m_data.end(); ++it) {

      // Add the pointer to the start of the next row
      while (current_row != it->first.at(0)) {
        current_row++;
        ptr.push_back(values.size());
      }

      // Add the value to values vector
      values.push_back(it->second);
      // Add the index of the column to indices vector
      indices.push_back(it->first.at(1));
    }
    // Add the final pointer for the last row
    ptr.push_back(values.size());
  } else if constexpr (Order == StorageOrder::ColumnMajor) {
    // Compress to CSC format
    // Order the m_data vector by columns
    std::map<std::array<std::size_t, 2>, T> col_data;
    for (size_t k = 0; k < cols; k++) {
      for (auto it = m_data.begin(); it != m_data.end(); ++it) {
        if (it->first.at(1) == k) {
          col_data[{k, it->first.at(0)}] = it->second;
        }
      }
    }
    // Now it's like the CSR code
    size_t current_col = 0;
    for (auto it = col_data.begin(); it != col_data.end(); ++it) {

      while (current_col != it->first.at(0)) {
        current_col++;
        ptr.push_back(values.size());
      }

      // Add the value to values vector
      values.push_back(it->second);
      // Add the index of the column to indices vector
      indices.push_back(it->first.at(1));
    }
    // Add the final pointer for the last row
    ptr.push_back(values.size());
  }

  // Clear map
  m_data.clear();

  // Set compression flag
  compression = true;
}

template <class T, StorageOrder Order>
void SparseMatrix<T, Order>::uncompress() {
  // Clear compressed data vectors
  values.clear();
  indices.clear();
  ptr.clear();
  //@note you should check if already uncompressed and in that case just return.
  // Moreover with this method you set the matrix to zero. You should transfer
  // back the data form the compressed format to the map. The user cannot expect the matrix be emptied becouse it has beed decompressed. 
  // Clear compression flag
  compression = false;
}

template <class T, StorageOrder Order>
bool SparseMatrix<T, Order>::is_compressed() const {
  return compression;
}

template <class T, StorageOrder Order>
void SparseMatrix<T, Order>::resize(size_t r, size_t c) {
  // Update
  rows = r;
  cols = c;
  // Clear
  m_data.clear();
  values.clear();
  indices.clear();
  ptr.clear();
  //@note You are not resizing here, you are clearing the matrix! You have not a
  //matrix filled of zero and you do not distinguish compresseed and
  // uncompressed case.
}

template <class T, StorageOrder Order>
void SparseMatrix<T, Order>::print_matrix() {

  if constexpr (Order == StorageOrder::RowMajor) {
    std::cout << "Matrix storage order: RowMajor" << std::endl;
  } else if constexpr (Order == StorageOrder::ColumnMajor) {
    std::cout << "Matrix storage order: ColumnMajor" << std::endl;
  }

  if (!compression) {
    std::cout << "Matrix is not compressed." << std::endl;
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        // Find the value in the map for the current position
        auto it = m_data.find({i, j});
        if (it != m_data.end()) {
          // If found, print the value
          std::cout << it->second << "\t";
        } else {
          // Otherwise, print 0 or an empty value to indicate the cell is empty
          std::cout
              << "0\t"; // or std::cout << ".\t"; to indicate an empty cell
        }
      }
      std::cout << std::endl;
    }
  } else {
    std::cout << "Values: ";
    for (const auto &value : values) {
      std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "Indices: ";
    for (const auto &index : indices) {
      std::cout << index << " ";
    }
    std::cout << std::endl;
    std::cout << "Ptr: ";
    for (const auto &p : ptr) {
      std::cout << p << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

template <class T, StorageOrder Order>
T SparseMatrix<T, Order>::operator()(std::size_t row, std::size_t col) const {
  if (row >= rows || col >= cols) {
    throw std::out_of_range("SparseMatrix::operator(): Index out of range");
  }

  if (compression) {
    for (size_t i = ptr[row]; i < ptr[row + 1]; ++i) {
      //! note if inner indexes are sorted you can use a binary search and
      //! speedup the search
      if (indices[i] == col) {
        return values[i];
      }
    }
    return 0;
  } else {
    auto it = m_data.find({row, col});
    if (it != m_data.end()) {
      return it->second;
    } else {
      return 0;
    }
  }
}

template <class T, StorageOrder Order>
T &SparseMatrix<T, Order>::operator()(std::size_t row, std::size_t col) {
  if (row >= rows || col >= cols) {
    throw std::out_of_range("SparseMatrix::operator(): Index out of range");
  }

  if (compression) {
    for (size_t i = ptr[row]; i < ptr[row + 1]; ++i) {
      if (indices[i] == col) {
        return values[i];
      }
    }
    throw std::runtime_error("SparseMatrix::operator(): Cannot insert new "
                             "elements in compressed state");
  } else {
    return m_data[{row, col}];
  }
}

template <typename T, StorageOrder Order>
template <typename U>
std::vector<U>
SparseMatrix<T, Order>::operator*(const std::vector<U> &vec) const {
  if (cols != vec.size()) {
    throw std::invalid_argument(
        "SparseMatrix::operator*: Matrix columns and vector size mismatch");
  }

  std::vector<T> result(rows, T()); // Initialize result vector with zeros

  if (compression) {
    //@note If ptr.size() is 0 you are in trouble. In that case you should
    //return an empty vector.
    // Or, if you want ot avoid problems with the subtraction of unsigned
    // integers you can use auto finish= ptr.size();
    // finish=(finish==0?0:finish-1;
    // for (std::size_t i = 0; i < finish; ++i) // For each row (col) of the
    // matrix
    for (std::size_t i = 0; i < ptr.size() - 1;
         ++i) // For each row (col) of the matrix
    {
      std::size_t inner_start = ptr[i]; // Find the values in said row (col)
      std::size_t inner_end = ptr[i + 1];
      if constexpr (Order == StorageOrder::RowMajor) {
        for (std::size_t j = inner_start; j < inner_end;
             ++j) // Iterate over such elements perform matrix vector
                  // computation
          result[i] += values[j] * vec[indices[j]];
      } else if constexpr (Order == StorageOrder::ColumnMajor) {
        for (std::size_t k = inner_start; k < inner_end;
             ++k) // Iterate through each non-zero element in the column
          result[indices[k]] +=
              values[k] *
              vec[i]; // Accumulate the result by multiplying the non-zero
                      // element with the corresponding element of the vector
      }
    }
  } else {
    for (const auto &item : m_data) {
      result[item.first[0]] += item.second * vec[item.first[1]];
    }
  }
  return result;
}

template <class T, StorageOrder Order>
SparseMatrix<T, Order>
SparseMatrix<T, Order>::from_matrix_market(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line[0] == '%')
      continue; // Skip comments
    break;
  }
  std::istringstream iss(line);
  size_t rows, cols, entries;
  iss >> rows >> cols >> entries;

  SparseMatrix<T, Order> matrix(rows, cols);
  if constexpr (Order == StorageOrder::RowMajor) {
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      size_t row, col;
      T value;
      iss >> row >> col >> value;
      matrix(row - 1, col - 1) = value; // Matrix Market format is 1-based
    }
  } else if constexpr (Order == StorageOrder::ColumnMajor) {
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      size_t row, col;
      T value;
      iss >> row >> col >> value;
      matrix(col - 1, row - 1) = value; // Matrix Market format is 1-based
    }
  }
  return matrix;
}

} // namespace algebra

#endif /* SPARSE_MATRIX_IMPL_HPP */
