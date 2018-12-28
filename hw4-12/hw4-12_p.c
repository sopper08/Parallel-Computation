#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "MyMPI.h"

#define n 1000000

double f(int i){
	double x;
	x = (double)i/(double)n;
	return 4.0/(1.0+x*x);
}

int main(int argc, char* argv[]){
	int low_value, high_value;
	int id, p;
	int i;
	double area, whole_area;

	MPI_Init(&argc,&argv);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&p);

	low_value = BLOCK_LOW(id,p,n/2);
	high_value = BLOCK_HIGH(id,p,n/2);
	// printf("This is %d %d : l_v: %d / h_v: %d\n",id,p,low_value,high_value);
	area = f(0)-f(n);
	for(i=low_value;i<=high_value;i++)
		area+=4.0*f(2*i-1)+2*f(2*i);
	area/=(3.0*n);
	MPI_Reduce(&area,&whole_area,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
	if(!id)
		printf("Approximation of pi: %13.11f\n",whole_area);
	return 0;
}
