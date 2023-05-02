#include "utils.h"

void saveParametersToFile(int netSize, double J, double B, long iters, long repeat);
void runProgram(int argc, char *argv[], int grid, double J, 
                double B, long iterations, long repeat);

#include "utils.h"

void saveParametersToFile(int netSize, double J, double B, long iters, long repeat);
void runProgram(int argc, char *argv[], int grid, double J,
                double B, long iterations, long repeat);

int main(int argc, char *argv[]) {
    // Only execute the console GUI for rank 0
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        double J, B;
        int netSize;
        long iters, repeat;

        std::cout << "Enter Net Size (multiple of 4): ";
        std::cin >> netSize;
        while (netSize % 4 != 0) {
            std::cout << "Invalid Net Size. Please enter a multiple of 4: ";
            std::cin >> netSize;
        }

        std::cout << "Enter J (-1 to 1): ";
        std::cin >> J;
        while (J < -1 || J > 1) {
            std::cout << "Invalid J. Please enter a value between -1 and 1: ";
            std::cin >> J;
        }

        std::cout << "Enter B (-1 to 1): ";
        std::cin >> B;
        while (B < -1 || B > 1) {
            std::cout << "Invalid B. Please enter a value between -1 and 1: ";
            std::cin >> B;
        }

        std::cout << "Enter number of iterations:";
        std::cin >> iters;
        while (iters <= 0) {
            std::cout << "Invalid number of iterations. Please enter a positive value: ";
            std::cin >> iters;
        }

        std::cout << "Enter number of repeats:";
        std::cin >> repeat;
        while (repeat <= 0) {
            std::cout << "Invalid number of repeats. Please enter a positive value: ";
            std::cin >> repeat;
        }

        saveParametersToFile(netSize, J, B, iters, repeat);

        std::string input;
        while (true) {
            std::cout << "Enter parameter name to edit ('start' to run the program): ";
            std::cin >> input;

            
            if (input == "netSize") {
                std::cout << "Enter new Net Size: ";
                std::cin >> netSize;
                while (netSize % 4 != 0) {
                    std::cout << "Invalid Net Size. Please enter a multiple of 4: ";
                    std::cin >> netSize;
                }
                saveParametersToFile(netSize, J, B, iters, repeat);
            } else if (input == "J") {
                std::cout << "Enter new J: ";
                std::cin >> J;
                while (J < -1 || J > 1) {
                    std::cout << "Invalid J. Please enter a value between -1 and 1: ";
                    std::cin >> J;
                }
                saveParametersToFile(netSize, J, B, iters, repeat);
            } else if (input == "B") {
                std::cout << "Enter new B: ";
                std::cin >> B;
                while (B < -1 || B > 1) {
                    std::cout << "Invalid value for B. Please enter a value between -1 and 1: ";
                    std::cin >> B;
                }
                saveParametersToFile(netSize, J, B, iters, repeat);
            } else if (input == "iters") {
                std::cout << "Enter new number of iterations: ";
                std::cin >> iters;
                while (iters <= 0) {
                    std::cout << "Invalid number of iterations. Please enter a positive value: ";
                    std::cin >> iters;
                }
                saveParametersToFile(netSize, J, B, iters, repeat);
            } else if (input == "repeat") {
                std::cout << "Enter new number of repeats: ";
                std::cin >> repeat;
                while (repeat <= 0) {
                    std::cout << "Invalid number of repeats. Please enter a positive value: ";
                    std::cin >> repeat;
                }
                saveParametersToFile(netSize, J, B, iters, repeat);
            } else if (input == "start") {
                runProgram(argc, argv, netSize, J, B, iters, repeat);
            } else {
                std::cout << "Invalid parameter name. Try again." << std::endl;
            }
        }
    }

    MPI_Finalize();
    return 0;
}


void runProgram(int argc, char *argv[], int grid, double J, 
                double B, long iterations, long repeat){
    for(int rep=0; rep<repeat; rep++){
        std::string dir_name = createFolderWithTimestampName();
        std::cout << "Running the program" << std::endl;

        if ( dir_name == "ERROR" ){
            std::cout << "ERROR during creating dir" << std::endl;
            return ;
        }

        // tu trzba pozmianić wartości żeby pasowało (tworzyło kwadtową siatkę) 
        int rank, num_procs;
        int row_size = 12;
        int rows_per_proc = 2;
        int iters = iterations;

        // MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

        // Inicjalizacja siatki spipnow
        int* cluster = generateSpins(rows_per_proc, row_size);
        MPI_Barrier(MPI_COMM_WORLD);
        int* recv_buffer = new int[ row_size * row_size ];
        MPI_Gather( cluster, row_size*rows_per_proc, MPI_INT, recv_buffer, 
                    row_size*rows_per_proc, MPI_INT, 0, MPI_COMM_WORLD);     
        if (rank == 0) {
            std::cout << "Initial state:" << std::endl;
            printVector2D( recv_buffer, row_size, row_size);
        }
        delete []cluster;
        
        //losoujemy wartosći
        //wysyłamy kopię tabl
        for(int i=0; i<iters; i++)
        {



            if( i%(iters/10) || i == iters-1 ){
                saveGrid( recv_buffer, row_size, i, dir_name);
            }

        }

        // Kończenie programu
        MPI_Finalize(); 
        delete[] recv_buffer;
    }
}

void saveParametersToFile(int netSize, double J, double B, long iters, long repeat) {
    std::ofstream file("parameters.txt");
    if (!file) {
        std::cout << "Failed to open file for writing." << std::endl;
        return;
    }

    file << "Net Size: " << netSize << std::endl;
    file << "J: " << J << std::endl;
    file << "B: " << B << std::endl;
    file << "Number of iterations" << iters << std::endl;
    file << "Number repeats" << repeat << std::endl;

    file.close();
    std::cout << "Parameters saved to file successfully." << std::endl;
}