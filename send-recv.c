// mpicc send-recv.c -o sendrecv && mpiexec sendrecv
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

int main(int argc, char** argv)
{
    // If you want to simulate with more items just change this variable
    int randomNumbersSize = 1000;
    int world_size, world_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


    int *randomNumbers;

    if (world_rank == 0)
    {
        srand(100);

        randomNumbers = generate_random_array(randomNumbersSize, 1, 100);

        // Calculate elements per process
        int lastElementIdx;
        int elementsPerProcess = randomNumbersSize / (world_size - 1);

        // Send the quantity and elements to the slaves
        for (int i=1; i<world_size - 1; i++) {
            lastElementIdx = elementsPerProcess * (i-1);

            MPI_Send(&elementsPerProcess, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&randomNumbers[lastElementIdx], elementsPerProcess, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
        // The last slave needs to receive the rest of the elements
        int lastWorldIdx = world_size - 1;
        lastElementIdx = elementsPerProcess * (lastWorldIdx - 1);
        int elementsLeft = randomNumbersSize - lastElementIdx;
        MPI_Send(&elementsLeft, 1, MPI_INT, lastWorldIdx, 0, MPI_COMM_WORLD);
        MPI_Send(&randomNumbers[lastElementIdx], elementsLeft, MPI_INT, lastWorldIdx, 0, MPI_COMM_WORLD);

        int finalSum = 0;

        // Receive the partial sums and store it into a final sum
        for(int i = 1; i < world_size; i++) {
            int receivedPartialSum = 0;
            MPI_Recv(&receivedPartialSum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            finalSum += receivedPartialSum;
        }

        printf("\nSoma final: %d\n", finalSum);
    } else {
        int receivedElementsQtd;
        int slaveRandomNumbers[randomNumbersSize];

        // Receive the elements from the master
        MPI_Recv(&receivedElementsQtd, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&slaveRandomNumbers, receivedElementsQtd, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Calculate partial sum
        int partialSum = 0;
        for(int i = 0; i < receivedElementsQtd; i++)
            partialSum += slaveRandomNumbers[i];
        
        // Send the result of the partial sum back to the master
        printf("Soma parcial %d: %d\n", world_rank, partialSum);
        MPI_Send(&partialSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}