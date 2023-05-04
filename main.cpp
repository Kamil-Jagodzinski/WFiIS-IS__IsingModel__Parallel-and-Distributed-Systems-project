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
    bool stay_in_GUI = true;
    readParametersFromFile( grid_size, J, B, iterations, repeat);

    if(rank == 0) {
    // Wyświetlenie GUI
        while(true && stay_in_GUI) {
            std::cout << "===================================================="<< std::endl;
            std::cout << "||         SELECT ACTION FROM LIST BELOW:         ||" << std::endl;
            std::cout << "|| 1. Change grid_size      4. Change iterations  ||" <<std::endl;
            std::cout << "|| 2.     Change J          5.   Change repeat    ||" << std::endl;
            std::cout << "|| 3.     Change B          6.    Run program     ||" << std::endl;
            std::cout << "||                                                ||" <<std::endl;
            std::cout << "||                                                ||" <<std::endl;
            std::cout << "===================================================="<< std::endl;
            std::cout << "                Currently set parameters:" << std::endl;
            std::cout << "grid_size = " << grid_size << ",\t J = " << J << ",\t B = " << B 
                    << ",\niterations = " << iterations << ",\nrepeat = " << repeat << std::endl;

            std::cout <<"\n>>  Choose option: \n>>";

            int option;
            std::cin >> option;

        // obsluga dostepnych akcji
        switch (option) {
            case 1:
                system("clear"); 
                std::cout << "Enter new Net Size: ";
                while (!(std::cin >> grid_size) || grid_size <= 0) {
                    std::cout << "Invalid input. Please enter a positive integer for Net Size: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                saveParametersToFile(grid_size, J, B, iterations, repeat);
                break;
            case 2:
                system("clear"); 
                std::cout << "Enter new J (-1 to 1): ";
                while (!(std::cin >> J) || J < -1 || J > 1) {
                    std::cout << "Invalid input. Please enter a value between -1 and 1 for J: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                saveParametersToFile(grid_size, J, B, iterations, repeat);
                break;
            case 3:
                system("clear"); 
                std::cout << "Enter new B (-1 to 1): ";
                while (!(std::cin >> B) || B < -1 || B > 1) {
                    std::cout << "Invalid input. Please enter a value between -1 and 1 for B: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                saveParametersToFile(grid_size, J, B, iterations, repeat);
                break;
            case 4:
                system("clear"); 
                std::cout << "Enter new iterations: ";
                while (!(std::cin >> iterations) || iterations <= 0) {
                    std::cout << "Invalid input. Please enter a positive integer for iterations: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                saveParametersToFile(grid_size, J, B, iterations, repeat);
                break;
            case 5:
                system("clear"); 
                std::cout << "Enter new repeat: ";
                while (!(std::cin >> repeat) || repeat <= 0) {
                    std::cout << "Invalid input. Please enter a positive integer for repeat: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                saveParametersToFile(grid_size, J, B, iterations, repeat);
                break;
            case 6:
                system("clear"); 
                std::cout << "Running the program" << std::endl;
                stay_in_GUI = false;
                break;
            default:
                std::cout << "Invalid option" << std::endl;
                break;
            }
        }
    }

    // Powstrzymanie innych procesów przed działaniem do momentu wyjścia z gui
    MPI_Barrier( MPI_COMM_WORLD );

    // wykonywanie obliczen
    runProgram(rank, num_procs, grid_size, J, B, iterations, repeat);
    
    // koniecznie programu
    MPI_Finalize(); 
    return 0;
}


void runProgram(int rank, int num_procs, int grid_size, double J, 
                double B, long long iterations, long long repeat){
    // inicjalizacja zmiennych 
    int row_size = grid_size;
    int iters = iterations;
    int rows_per_proc = row_size/num_procs;
    std::string dir_name;
    std::mt19937 gen( MPI_PROC_NULL + rank );
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    std::uniform_int_distribution<int> disInt(0, INT_MAX);

    for(int rep=0; rep<repeat; rep++){

        // tworzenie katalogu na wyniki
        if( rank == 0){
            dir_name = createFolderWithTimestampName(rep);
            if ( dir_name == "ERROR" ){
               std::cout << "ERROR during creating dir" << std::endl;
                return ;
            }
        }

        // inicjalizacja siatki spipnow
        int* cluster = generateSpins(rows_per_proc, row_size, rank);
        int* recv_buffer = new int[ row_size * row_size ];

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&J, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&B, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&grid_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&iterations, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
        MPI_Bcast(&repeat, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);


        // laczenie fragmentów siatki w jedną i przekazanie jej do kazdego procesu
        MPI_Allgather(  cluster, row_size * rows_per_proc, MPI_INT, recv_buffer,
                        row_size * rows_per_proc, MPI_INT, MPI_COMM_WORLD);

        
        for(int i=0; i<iters+num_procs; i+=num_procs)
        {
            // losowanie spinu dla danego procesu
            int idx = disInt(gen) % ( rows_per_proc * row_size ) + rank * rows_per_proc * row_size;

           
            double delta = calculateEnergyChange(recv_buffer, idx, row_size, rows_per_proc, num_procs);
            double p = 0.0;

            // decydowanie czy zostawiamy zmianę
            if( delta < 0.0 ){
                p = 1.0;
            } else{
                p = exp( -delta ); // zkaładmy kT = 1
            }

            if( dis(gen) < p ){
                 flipSpin(recv_buffer, idx);
            }
            std::copy( recv_buffer + rank *  row_size * rows_per_proc,
                            recv_buffer + (rank+1) * row_size * rows_per_proc - 1, 
                            cluster);

            // czekanie aż procesy sprawdza czy zmienic wartosc spinu 
            // i ponowne laczenie tablic w jedna
            MPI_Barrier(MPI_COMM_WORLD);
            MPI_Allgather(  cluster, row_size * rows_per_proc, MPI_INT, recv_buffer,
                            row_size * rows_per_proc, MPI_INT, MPI_COMM_WORLD);

            
            // zapis siatek, enegii i magenetyzmu do plikow
            if( rank == 0 && ( i == iters-1 || i%(iters/10) < num_procs) ){
                saveGrid( recv_buffer, row_size, dir_name);
            }

            if( rank == 0 && ( i == iters-1 || i%(iters/100) < num_procs) ){
                saveEnergy( energy(recv_buffer, J, B, row_size), dir_name );
                saveMag( avgMagnetism(recv_buffer, row_size * rows_per_proc * num_procs), dir_name );
            }
   
        }

        // zwalanianie pamieci
        delete[] cluster;
        delete[] recv_buffer;
    }
}
