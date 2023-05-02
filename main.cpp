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
    int row_size = grid_size;
    int iters = iterations;
    int rows_per_proc = row_size/num_procs;
    std::string dir_name;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    for(int rep=0; rep<repeat; rep++){
        if( rank == 0){
            dir_name = createFolderWithTimestampName(rep);
            if ( dir_name == "ERROR" ){
               std::cout << "ERROR during creating dir" << std::endl;
                return ;
            }
        }

        // Inicjalizacja siatki spipnow
        int* cluster = generateSpins(rows_per_proc, row_size);
        MPI_Barrier(MPI_COMM_WORLD);
        int* recv_buffer = new int[ row_size * row_size ];

        MPI_Allgather(  cluster, row_size * rows_per_proc, MPI_INT, recv_buffer,
                        row_size * rows_per_proc, MPI_INT, MPI_COMM_WORLD);

        /*
        if (rank == 0) {
            std::cout << "Initial state:" << std::endl;
            printVector2D( recv_buffer, row_size, row_size);
        }
        */
       
        
        for(int i=0; i<iters+num_procs; i+=num_procs)
        {
            
            // Calculate the energy of the spin at the given index in process 0
            double energy_before = 0.0, energy_after = 0.0;
            if(rank == 0) {
                energy_before = energy(recv_buffer, J, B, row_size);
            }
            // Broadcast the energy to all processes
            MPI_Bcast(&energy_before, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            // licz energię przed dla każdego i roześlij wartość,
            int idx = getRandomSpin(rank, rows_per_proc, row_size);

            // sprawdź energię po i czy zamienić wartości
            int* new_grid = nullptr;
            new_grid = flipSpin(recv_buffer, idx, row_size, rows_per_proc, num_procs);
            energy_after = energy(new_grid, J, B, row_size);

            double delta = energy_after - energy_before;

            double p = 0.0;
            if( delta < 0.0){
                p = 1.0;
            } else{
                p = exp( -delta ); // zkaładmy kT = 1
            }

            if( dis(gen) < p ){
                 std::copy( new_grid + rank *  row_size * rows_per_proc,
                            new_grid + (rank+1) * row_size * rows_per_proc - 1, 
                            cluster);
            } else {
                std::copy(  recv_buffer + rank *  row_size * rows_per_proc,
                            recv_buffer + (rank+1) * row_size * rows_per_proc - 1, 
                            cluster);
            }

            MPI_Barrier(MPI_COMM_WORLD);
            MPI_Allgather(  cluster, row_size * rows_per_proc, MPI_INT, recv_buffer,
                            row_size * rows_per_proc, MPI_INT, MPI_COMM_WORLD);

            if( rank == 0 && ( i == iters-1 || i%(iters/10) < num_procs) ){
                saveGrid( recv_buffer, row_size, i, dir_name);
            }

            if(new_grid){
                delete[] new_grid;
            }    
        }

        // Kończenie programu
        delete[] cluster;
        delete[] recv_buffer;
    }
}