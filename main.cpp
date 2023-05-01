#include <stdio.h>
#include <mpi.h>
#include "utils.h"

typedef std::vector<std::vector<double>> vector2d;

int main(int argc, char *argv[]) {
    int rank, num_procs;
    int rows_per_proc = 1;

    // określamy rozmiar wektora pojedyńczego wiersza
    int row_size = rows_per_proc * row_length;
    // określamy rozmiar wektorów w procescie
    int total_size = row_size * rows_per_proc;
    

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);


    // tworzymy tablice wejściową
    vector2d row = generateSpins(rank, num_procs, rows_per_proc, MPI_COMM_WORLD);
    

    if (rank == 0) {
        vector2d spins_all;
        spins_all.resize(rows_per_proc * num_procs, std::vector<double>(rows_per_proc * num_procs));
    }

    MPI_Gather( &spins_all[0][0], 
                rows_per_proc * rows_per_proc, 
                MPI_DOUBLE, &spins_all[rank * rows_per_proc][0], 
                rows_per_proc * rows_per_proc, 
                MPI_DOUBLE, 
                0, 
                MPI_COMM_WORLD);

    //



    printVector2D(spins_all);
    MPI_Finalize(); 
    return 0;
}

