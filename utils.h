#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <mpi.h>
typedef std::vector<std::vector<double>> vector2d;

vector2d generateSpins(int rank, int num_procs, int rows_per_proc, MPI_Comm comm);

void printVector2D(const vector2d vec);

void printVector2D(const double* vec, int rows, int row_size);