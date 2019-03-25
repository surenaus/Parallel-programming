#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 4
int main(int argc, char *argv[])
{
    int rank, size;     // for storing this process' rank, and the number of processes
    int *sendcounts;    // array describing how many elements to send to each process
    int *displs;        // array describing the displacements where each segment begins

    int rem = (SIZE*SIZE)%size; // elements remaining after division among processes
    int sum = 0;                // Sum of counts. Used to calculate displacements
    char rec_buf[100];          // buffer where the received data should be stored
	// the data to be distributed
    char data[SIZE][SIZE] = {
        {'a', 'b', 'c', 'd'},
        {'e', 'f', 'g', 'h'},
        {'i', 'j', 'k', 'l'},
        {'m', 'n', 'o', 'p'}
    };
   
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    sendcounts = (int *)malloc(sizeof(int)*size);
    displs = (int *)malloc(sizeof(int)*size);

    // calculate send counts and displacements
    for (int i = 0; i < size; i++) {
        sendcounts[i] = (SIZE*SIZE)/size;
        if (rem > 0) {
            sendcounts[i];
            rem--;
        }

        displs[i] = sum;
        sum += sendcounts[i];
    }
    if(rank == 0) {
	    for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE; j++)
		    printf("%i: %c\t", i,data[i][j]);
	    	printf("\n");
	    }
    }
    // print calculated send counts and displacements for each process
    if (0 == rank) {
        for (int i = 0; i < size; i++) {
            printf("sendcounts[%d] = %d\tdispls[%d] = %d\n", i, sendcounts[i], i, displs[i]);
        }
    }

    // divide the data among processes as described by sendcounts and displs

    int n = size;
    int m = size;
    char data2[SIZE][SIZE];

    int count_vertical = 0;
    for(int i = 0; i < n/2; i++) 
	for(int j = 0; j < m/2; j++) {
		data2[0][count_vertical] = data[i][j];
		count_vertical++;
    }
    count_vertical = 0;
    for(int i = n/2; i < n; i++) 
	for(int j = 0; j < m/2; j++) {
		data2[1][count_vertical] = data[i][j];
		count_vertical++;
    }
    count_vertical = 0;
    for(int i = 0; i < n/2; i++) 
	for(int j = m/2; j < m; j++) {
		data2[2][count_vertical] = data[i][j];
		count_vertical++;
    }
    count_vertical = 0;
    for(int i = n/2; i < n; i++) 
	for(int j = m/2; j < m; j++) {
		data2[3][count_vertical] = data[i][j];
		count_vertical++;
    }
    MPI_Scatterv(&data2, sendcounts, displs, MPI_CHAR, &rec_buf, 100,    MPI_CHAR, 0, MPI_COMM_WORLD);
    // print what each process received
    printf("%d: ", rank);
    for (int i = 0; i < sendcounts[rank]; i++) 
        printf("%c\t", rec_buf[i]);
    printf("\n");

    MPI_Finalize();

    free(sendcounts);
    free(displs);

    return 0;
}