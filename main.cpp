#include "utils.h"

void runProgram(int rank, int num_procs, int grid_size, double J, 
                double B, long long iterations, long long repeat);

int main(int argc, char *argv[]) {
    int rank, num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    int grid_size; 
    double J, B; 
    long long iterations, repeat;
    bool stay_in_GUI = true, exit_run = false;
    readParametersFromFile( grid_size, J, B, iterations, repeat);

    if(rank == 0) {
    // Wyświetlenie GUI
        while(true && stay_in_GUI) {
            std::cout << "===================================================="<< std::endl;
            std::cout << "||            SELECT ACTION FROM LIST BELOW:      ||" << std::endl;
            std::cout << "|| 1. Change grid_size      4. Change iterations  ||" <<std::endl;
            std::cout << "|| 2. Change J              5. Change repeat      ||" << std::endl;
            std::cout << "|| 3. Change B              6. Run program        ||" << std::endl;
            std::cout << "||                                                ||" <<std::endl;
            std::cout << "|| 0. Exit                                        ||" <<std::endl;
            std::cout << "||                                                ||" <<std::endl;
            std::cout << "===================================================="<< std::endl;
            std::cout << "                Currently set parameters:" << std::endl;
            std::cout << "grid_size = " << grid_size << ",\t J = " << J << ",\t B = " << B 
                    << ",\niterations = " << iterations << ",\nrepeat = " << repeat << std::endl;

            std::cout <<"\n>>  Choose option: \n>>";

            int option;
            std::cin >> option;

            switch(option) {
                case 1:
                    system("clear"); 
                    std::cout << "Enter new Net Size: ";
                    std::cin >> grid_size;
                    saveParametersToFile(grid_size, J, B, iterations, repeat);
                    break;
                case 2:
                    system("clear"); 
                    std::cout << "Enter new J: ";
                    std::cin >> J;
                    saveParametersToFile(grid_size, J, B, iterations, repeat);
                    break;
                case 3:
                    system("clear"); 
                    std::cout << "Enter new B: ";
                    std::cin >> B;
                    saveParametersToFile(grid_size, J, B, iterations, repeat);
                    break;
                case 4:
                    system("clear"); 
                    std::cout << "Enter new iterations: ";
                    std::cin >> iterations;
                    saveParametersToFile(grid_size, J, B, iterations, repeat);
                    break;
                case 5:
                    system("clear"); 
                    std::cout << "Enter new repeat: ";
                    std::cin >> repeat;
                    saveParametersToFile(grid_size, J, B, iterations, repeat);
                    break;
                case 6:
                    system("clear"); 
                    std::cout << "Running the program" << std::endl;
                    stay_in_GUI = false;
                    break;
                case 0:
                    system("clear"); 
                    std::cout << "Leaving the program" << std::endl;
                    stay_in_GUI = false;
                    exit_run = true;
                    break;
                default:
                    std::cout << "Invalid option" << std::endl;
                    break;
                    }
                }
            }

    MPI_Barrier( MPI_COMM_WORLD );

    if(exit_run){
        MPI_Finalize(); 
        return 0;
    }

    runProgram(rank, num_procs, grid_size, J, B, iterations, repeat);
    MPI_Finalize(); 
    return 0;
}


void runProgram(int rank, int num_procs, int grid_size, double J, 
                double B, long long iterations, long long repeat){
    for(int rep=0; rep<repeat; rep++){

        std::string dir_name;
        if( rank == 0){
            dir_name = createFolderWithTimestampName(rep);
            if ( dir_name == "ERROR" ){
               std::cout << "ERROR during creating dir" << std::endl;
                return ;
            }
        }
        
        int row_size = grid_size;
        int iters = iterations;
        int rows_per_proc = row_size/num_procs;

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
        
        for(int i=0; i<iters; i++)
        {

            if( rank == 0 && ( i%(iters/1000) == 0 || i == iters-1 ) ){
                saveGrid( recv_buffer, row_size, i, dir_name);
            }

        }

        // Kończenie programu
        delete[] cluster;
        delete[] recv_buffer;
    }
}