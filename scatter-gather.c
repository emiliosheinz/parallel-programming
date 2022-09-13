// mpicc scatter-gather.c -o scattergether && mpiexec scattergether
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Generates random int between two numbers
int random_in_range (int low, int high)
{
   return (rand() % (high-low+1)) + low;
}

// Generates a random array of int of a specific size
int * generate_random_array (int size, int low, int high)
{
   int i, rand_num, *intvec;
   intvec = (int*) malloc(sizeof(int)*size);
   for (i=0; i<size; i++)
   {
      intvec[i] = random_in_range(low,high);
   }
   return intvec;
}

int computArraySum(int *array, int size) {
    int sum = 0;

    for (int i=0; i<size; i++)
    {
        sum += array[i];
    }

    return sum;
}

int main(int argc, char** argv)
{
    // If you want to simulate with more items just change this variable
    int randomNumbersSize = 1000;
    int world_size, world_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Rank 0 generates random numbers array
    int *randomNumbers;
    if (world_rank == 0)
    {
        srand(100);
        randomNumbers = generate_random_array(randomNumbersSize, 1, 1000);
    }

    // We calculate de elements per process
    // This calc is buggy when the size of the array and the number of processes are not divisible
    int elementsPerProcess = randomNumbersSize / world_size;;

    // I've tried to change the elements per processes of the rank 0 processes to take all the 
    // remaining items, but it throws an error.
    // if(world_rank == 0) {
    //     elementsPerProcess = randomNumbersSize - (elementsPerProcess * (world_size - 1));
    // } 

    // Scatter the numbers bettwen the processes
    int *scatterRandomNumbers = malloc(sizeof(int) * elementsPerProcess);
    MPI_Scatter(randomNumbers, elementsPerProcess, MPI_INT, scatterRandomNumbers, elementsPerProcess, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the partial sum
    int partialSum = computArraySum(scatterRandomNumbers, elementsPerProcess);

    int *partialSums;
    if(world_rank == 0) {
        partialSums = malloc(sizeof(int) * world_size);
    }

    // Gather the partial sums
    MPI_Gather(&partialSum, 1, MPI_INT, partialSums, 1, MPI_INT, 0, MPI_COMM_WORLD);

     
    // Let the master process calculate the final sum
    if(world_rank == 0) { 
        int finalSum = computArraySum(partialSums, world_size);

        printf("Soma final:  %d\n", finalSum);
    }

    MPI_Finalize();
    return 0;
}