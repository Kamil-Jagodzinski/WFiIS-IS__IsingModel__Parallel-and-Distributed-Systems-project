#include "utils.h"

void printVector2D(const vector2d vec) {
    std::cout << "Model grid:" << std::endl;
    for (const auto& row : vec) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

vector2d generateSpins(int rank, int num_procs, int rows_per_proc, MPI_Comm comm) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    vector2d spins;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &num_procs);
    double probability = 0.5;
    int row_length = rows_per_proc * num_procs;

    for (int i = 0; i < rows_per_proc; i++) {
        std::vector<double> temp;
        for (int j = 0; j < row_length; j++) {
            double rand = dis(gen);
            if (rand > probability) {
                temp.push_back(1);
            } else {
                temp.push_back(0);
            }
        }
        spins.push_back(temp);
    }

    return spins;
}
