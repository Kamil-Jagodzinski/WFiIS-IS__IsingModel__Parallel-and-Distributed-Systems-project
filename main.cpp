#include <stdio.h>
#include <mpi.h>
#include "utils.h"

using vector2d = std::vector<std::vector<double>>;
using vector = std::vector<double>;

int main(int argc, char *argv[]) {
    int rank, num_procs;
    int row_size = 6;
    int rows_per_proc = 6;
    vector2d spins_all;

    // określamy rozmiar wektora pojedyńczego wiersza
    // określamy rozmiar wektorów w procescie
    int total_size = row_size * rows_per_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // tworzymy tablice wejściową
    vector2d row = generateSpins(rank, num_procs, rows_per_proc, MPI_COMM_WORLD);
    std::cout << rank << "/" << num_procs << std::endl;
    // printVector2D(row);
    vector buffer;
    vector recv_buffer;

    MPI_Barrier(MPI_COMM_WORLD);

    if( rank == 0){
        std::cout << "|| AFTER MPI_Barrier" << std::endl;
    }

    MPI_Gather( row.data(), row_size, MPI_DOUBLE, recv_buffer.data(), 
                row_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printVector2D( recv_buffer.data(), row_size, row_size);
    }

    MPI_Finalize(); 
    return 0;
}

