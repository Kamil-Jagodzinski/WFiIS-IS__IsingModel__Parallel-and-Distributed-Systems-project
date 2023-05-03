# WFiIS-IS IsingModel
## Project Description
The project aims to implement a distributed simulation of the Ising model using the Metropolis algorithm in C++ with the MPI library. 
The Ising model is a physical model that describes the interaction of spins in a crystalline lattice.

## Requirements
To run the project, the following must be installed:
- C++11
- MPI
- optionally: Python (with NumPy and Matplotlib libraries)

## Running
Clone the repository to your home directory, navigate to it, and compile the file:
In the bash console:

```
git clone https://github.com/Kamil-Jagodzinski/WFiIS-IS__IsingModel__Parallel-and-Distributed-Systems-project.git
cd WFiIS-IS__IsingModel__Parallel-and-Distributed-Systems-project
```

In the Makefile, uncomment the appropriate flag for our call. The difference results from the different location of 
MPI in the computer lab than the default location when downloading MPI from the console.

```
make
mpiexec -n <number of processes> ./ising.out // locally
mpiexec -f nodes -n <number of processes> ./ising.out // for the computer lab
```

After running the file, a simple terminal interface (UI) will open up. From the UI, the user can choose:
- size of the spin lattice,
- parameter J, which determines the strength of the interaction between spins,
- parameter B, which determines the strength of the external magnetic field,
- number of iterations in the simulation, and
- number of times the simulation should be repeated.

```diff 
! Size of the spin lattice have to be multiple of number of processes 
```

