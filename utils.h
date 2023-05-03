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
#include <bits/stdc++.h>


/**
 * @brief Prints a 2D vector.
 * @param vec A pointer to the vector to print.
 * @param rows The number of rows in the vector.
 * @param row_size The size of each row in the vector.
 */
void printVector2D(const int* vec, int rows, int row_size);


/**
 * @brief Generates a 2D grid of random Ising model spins.
 *        This function generates a 2D grid of random Ising model spins using a uniform
 *        distribution with a probability of 0.5 for each spin. The spins are stored in
 *        row-major order in a dynamically allocated array.
 * @param rows_per_proc The number of rows of spins to generate for each MPI process.
 * @param row_size The number of columns of spins in each row.
 * @param rank Procces number.
 * @return A pointer to the dynamically allocated array containing the generated spins.
 */
int* generateSpins(int rows_per_proc, int row_size, int rank);


/**
 * @brief Calculates the energy of a single spin in the Ising model.
 * @param index The index of the spin.
 * @param grid A pointer to the grid of spins.
 * @param row_size The size of each row in the grid.
 * @param J The coupling constant.
 * @param B The magnetic field strength.
 * @return The energy of the spin.
 */
double single_spin_energy(int index, const int* grid, int row_size, double J, double B);


/**
 * @brief Calculates the total energy of a 2D grid of spins using the Ising model.
 * @param grid A pointer to the 2D grid of spins.
 * @param J The interaction strength parameter.
 * @param B The magnetic field strength parameter.
 * @param row_size The size of each row in the grid.
 * @return The total energy of the grid.
 */
double energy(int* grid, double J, double B, int row_size);


/**
 * @brief Saves the spin grid to a text file.
 * @param grid A pointer to the spin grid to save.
 * @param row_size The size of each row in the grid.
 * @param folderName The name of the folder to save the file in.
 */
void saveGrid(int* grid, int row_size, std::string folderName);


/**
 * @brief Saves the average magnetism to a file.
 * @param mg The average magnetism to save.
 * @param folderName The name of the folder where the file will be saved.
 */
void saveMag(double mg, std::string folderName);


/**
 * @brief Saves the energy to a file.
 * @param energy The value of the energy to be saved.
 * @param folderName The name of the folder in which to save the file.
 */
void saveEnergy(double energy, std::string folderName);


/**
 * @brief Creates a new folder with a timestamp-based name in the "result" 
 *        directory. The name of the folder is generated using the 
 *        current system time and an optional repetition number.
 * @param rep An optional repetition number to be included in the folder name.
 * @return The name of the newly created folder, or "ERROR" if folder creation failed.
 */
std::string createFolderWithTimestampName(int rep);


/**
 * @brief Saves the simulation parameters to a file.
 * @param netSize The size of the network used in the simulation.
 * @param J The value of the J parameter used in the simulation.
 * @param B The value of the B parameter used in the simulation.
 * @param iters The number of iterations used in the simulation.
 * @param repeat The number of times the simulation was repeated.
 */
void saveParametersToFile(int netSize, double J, double B, long long iters, long long repeat);


/**
 * @brief Reads simulation parameters from a file and stores them in the 
 *        provided variables. The function reads the values of the 
 *        net size, J, B, number of iterations, and number of repeats from 
 *        file and stores them in the variables provided as arguments.
 * @param netSize A reference to a variable where the net size will be stored.
 * @param J A reference to a variable where the J coupling constant will be stored.
 * @param B A reference to a variable where the external magnetic field B will be stored.
 * @param iters A reference to a variable where the number of iterations will be stored.
 * @param repeat A reference to a variable where the number of repeats will be stored.
 */
void readParametersFromFile(int& netSize, double& J, double& B, long long& iters, long long& repeat);


/**
 * @brief Flips the spin of a grid element at a given index.
 * @param grid The original grid.
 * @param idx The index of the element to flip.
 */
void flipSpin(int* grid, int idx);


/**
 * @brief Calculates the average magnetism of a spin array.
 * @param spinArray An integer array containing the spin values.
 * @param spinArraySize The size of the spin array.
 * @return The average magnetism value.
*/
double avgMagnetism(int* spinArray, int spinArraySize);


double calculateEnergyChange(int* grid, int idx, int row_size, int rows_per_proc, int num_proc);