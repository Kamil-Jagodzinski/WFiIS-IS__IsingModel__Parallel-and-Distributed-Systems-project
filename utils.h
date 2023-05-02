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
#include <cstdlib> 

int* generateSpins(int rows_per_proc, int row_size);

void printVector2D(const int* vec, int rows, int row_size);

double energy(int* grid, double J, double B, int row_size);

double single_spin_energy(int index, const int* grid, int row_size, double J, double B);

void saveGrid(int* grid, int row_size, std::string folderName);

std::string createFolderWithTimestampName(int rep);

void saveParametersToFile(int netSize, double J, double B, long long iters, long long repeat);

void readParametersFromFile(int& netSize, double& J, double& B, long long& iters, long long& repeat);

int getRandomSpin(int rank, int rows_per_proc, int row_size);

int* flipSpin(int* grid, int idx, int row_size, int rows_per_proc, int num_proc);

double avgMagnetism(int* spinArray, int spinArraySize);

void saveMag(double mg, std::string folderName);

void saveEnergy(double energy, std::string folderName);
