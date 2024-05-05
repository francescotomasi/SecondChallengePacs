#include "sparse_matrix.hpp"
#include <iostream>
#include <vector>
#include <chrono>

int main() {
    using namespace algebra;
    using namespace std::chrono;

    // Leggi la matrice dal file Matrix Market
    SparseMatrix<double, StorageOrder::ColumnMajor> matrix = SparseMatrix<double, StorageOrder::ColumnMajor>::from_matrix_market("lnsp_131.mtx");

    // Genera un vettore della dimensione corretta
    std::vector<double> vec(matrix.get_cols(), 1.0);

    // Esegui e cronometra la moltiplicazione matrice-vettore per la matrice non compressa
    auto start = high_resolution_clock::now();
    std::vector<double> result = matrix * vec;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time for uncompressed matrix-vector multiplication: " << duration.count() << " microseconds" << std::endl;


    // Compression
    auto start_compress = std::chrono::steady_clock::now();
    matrix.compress();
    auto end_compress = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> compress_time = end_compress - start_compress;
    std::cout << "Compression time: " << compress_time.count() << " microseconds" << std::endl;


    // Esegui e cronometra la moltiplicazione matrice-vettore per la matrice compressa
    start = high_resolution_clock::now();
    result = matrix * vec;
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time for compressed matrix-vector multiplication: " << duration.count() << " microseconds" << std::endl;

    return 0;
}


