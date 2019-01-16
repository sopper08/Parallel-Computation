#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"

void process0();
void process1();
const int sample = 100000;

int main(int argc, char* argv[]){
	int id;
	int p;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Barrier(MPI_COMM_WORLD);
	
	if(id==0) process0();
	if(id==1) process1();

	MPI_Finalize();
	return 0;
}

void process0(){
	int i;
	double avg_elapsed_time;
	int lengthOfTestMsg;

	for(lengthOfTestMsg=500;lengthOfTestMsg<=2000;lengthOfTestMsg+=100){
		avg_elapsed_time = 0;
		for(i=0;i<sample;i++){
			MPI_Status status;
			int* testMsg;
			double elapsed_time;
	
			testMsg = (int*)malloc(lengthOfTestMsg*sizeof(int));
			elapsed_time = -MPI_Wtime();
			MPI_Send(
					testMsg,
					lengthOfTestMsg,
					MPI_INT,
					1,
					i,
					MPI_COMM_WORLD);
			MPI_Recv(
					testMsg,
					lengthOfTestMsg,
					MPI_INT,
					1,
					i,
					MPI_COMM_WORLD,
					&status);
			elapsed_time += MPI_Wtime();
			avg_elapsed_time += elapsed_time;
			//printf("Finish! Execution time: %10.20f\n",elapsed_time);
			free(testMsg);
		}
		avg_elapsed_time /= sample;
		printf("In testMsg = %d Bytes, Average execution time: %10.20f\n",lengthOfTestMsg,avg_elapsed_time);
	}
}

void process1(){
	int i;
	int lengthOfTestMsg;

	for(lengthOfTestMsg=500;lengthOfTestMsg<=2000;lengthOfTestMsg+=100){
		for(i=0;i<sample;i++){
			MPI_Status status;
			int* testMsg;
			testMsg = (int*)malloc(lengthOfTestMsg*sizeof(int));
			MPI_Recv(
					testMsg,
					lengthOfTestMsg,
					MPI_INT,
					0,
					i,
					MPI_COMM_WORLD,
					&status);
			MPI_Send(
					testMsg,
					lengthOfTestMsg,
					MPI_INT,
					0,
					i,
					MPI_COMM_WORLD);
			free(testMsg);
		}
	}
}
