#include "utils.h"

void saveParametersToFile(int netSize, double J, double B, long iters, long repeat);
void runProgram(int argc, char *argv[], int grid, double J, 
                double B, long iterations, long repeat);

int main(int argc, char *argv[]) {
    double J, B;
    int netSize;
    long iters, repeat;

    std::cout << "Enter Net Size (multiple of "<< &argv <<"): ";
    std::cin >> netSize;

    std::cout << "Enter J (-1 to 1): ";
    std::cin >> J;

    std::cout << "Enter B (-1 to 1): ";
    std::cin >> B;

    std::cout << "Enter number of iterations:";
    std::cin >> iters;

    std::cout << "Enter number of repeats:";
    std::cin >> repeat;

    saveParametersToFile(netSize, J, B, iters, repeat);

    std::string input;
    while (true) {
        std::cout << "Enter parameter name to edit ('start' to run the program): ";
        std::cin >> input;

        if (input == "netSize") {
            std::cout << "Enter new Net Size: ";
            std::cin >> netSize;
            saveParametersToFile(netSize, J, B, iters, repeat);
        } else if (input == "J") {
            std::cout << "Enter new J: ";
            std::cin >> J;
            saveParametersToFile(netSize, J, B, iters, repeat);
        } else if (input == "B") {
            std::cout << "Enter new B: ";
            std::cin >> B;
            saveParametersToFile(netSize, J, B, iters, repeat);
        } else if (input == "iters") {
            std::cout << "Enter new number of iterations: ";
            std::cin >> iters;
            saveParametersToFile(netSize, J, B, iters, repeat);
        } else if (input == "repeat") {
            std::cout << "Enter new number of repeat: ";
            std::cin >> repeat;
            saveParametersToFile(netSize, J, B, iters, repeat);
        } else if (input == "start") {
            //
            // Wczytać dane do wywołnia
            //
            runProgram(argc, argv, netSize, J, B, iters, repeat);
        } else {
            std::cout << "Invalid parameter name. Try again." << std::endl;
        }
    }

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

        MPI_Init(&argc, &argv);
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

    MPI_Gather( row.data(), row_size, MPI_DOUBLE, recv_buffer.data(), 
                row_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printVector2D( recv_buffer.data(), row_size, row_size);
    }

    MPI_Finalize(); 
    return 0;
}

