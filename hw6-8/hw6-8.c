#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"

void process0(int);
void process1(int);

int main(int argc, char* argv[]){
	int id;
	int p;
	int length;
	
	length = 1;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Barrier(MPI_COMM_WORLD);
	
	if(id==0) process0(10);
	if(id==1) process1(10);




	MPI_Finalize();
	return 0;
}

void process0(int lengthOfTestMsg){
	MPI_Status status;
	char* testMsg;
	double elapsed_time;

	testMsg = (char*)malloc(lengthOfTestMsg*sizeof(char));
	elapsed_time = -MPI_Wtime();
	MPI_Send(
			testMsg,
			lengthOfTestMsg,
			MPI_CHAR,
			1,
			1,
			MPI_COMM_WORLD);
	MPI_Recv(
			testMsg,
			lengthOfTestMsg,
			MPI_CHAR,
			1,
			1,
			MPI_COMM_WORLD,
			&status);
	elapsed_time += MPI_Wtime();
	printf("Finish! Execution time: %10.20f\n",elapsed_time);
}

void process1(int lengthOfTestMsg){
	MPI_Status status;
	char* testMsg;
	testMsg = (char*)malloc(lengthOfTestMsg*sizeof(char));
	MPI_Recv(
			testMsg,
			lengthOfTestMsg,
			MPI_CHAR,
			0,
			1,
			MPI_COMM_WORLD,
			&status);
	MPI_Send(
			testMsg,
			lengthOfTestMsg,
			MPI_CHAR,
			0,
			1,
			MPI_COMM_WORLD);
}
