#pragma once

#include <iostream>
#include <random>
#include <mpi.h>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

int* generateSpins(int rows_per_proc, int row_size);

void printVector2D(const int* vec, int rows, int row_size);

double energy(const int* grid, double J, double B, int row_size);

double single_spin_energy(int index, const int* grid, int row_size, double J, double B);

void saveGrid(int* grid, int row_size, int iteration, std::string folderName);

std::string createFolderWithTimestampName();
