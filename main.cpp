#include <stdio.h>
#include <vector>
#include <mpi.h>
#include "utils.h"

using vector2d = std::vector<std::vector<double>>;
using vector = std::vector<double>;

int main(int argc, char* argv[]) {
    int rank, num_procs;
    int row_size = 12;
    int rows_per_proc = 2;
    vector2d spins_all;

    int total_size = row_size * rows_per_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    vector2d row = generateSpins(rank, num_procs, rows_per_proc, MPI_COMM_WORLD);

    vector buffer;
    vector recv_buffer(total_size); // Resize recv_buffer to accommodate the gathered data

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "|| AFTER MPI_Barrier" << std::endl;
    }

    MPI_Gather(row.data(), row_size, MPI_DOUBLE, recv_buffer.data(), row_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printVector2D(recv_buffer.data(), row_size, row_size);
    }

    MPI_Finalize();
    return 0;
}
