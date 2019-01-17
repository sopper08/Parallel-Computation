#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "MyMPI.h"

int main(int argc, char *argv[]){
	int** a;
	int*  storage;
	int i,j,k;
	int m,n;
	int p;
	int id;
	int iteration;
	int print_iteration;
	double elapsed_time;

	void compute_this_iteration(int,int,int**,int,int,int,int);
	
	iteration = strtol(argv[2],NULL,10);
	print_iteration = strtol(argv[3],NULL,10);
	//printf("iteration: %d\n",iteration);
	//printf("print_iteration: %d\n",print_iteration);

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	read_row_striped_matrix(argv[1],(void*) &a,(void*) &storage,MPI_INT,&m,&n,MPI_COMM_WORLD);
	//printf("m: %d\n",m);
	//printf("n: %d\n",n);
	
	print_row_striped_matrix((void**)a,MPI_INT,m,n,MPI_COMM_WORLD);
	
	MPI_Barrier(MPI_COMM_WORLD);
	elapsed_time = -MPI_Wtime();
	compute_this_iteration(id,p,(int**)a,m,n,iteration,print_iteration);
	elapsed_time += MPI_Wtime();
	if(!id) printf("Execution time: %6.10f",elapsed_time);

	//print_row_striped_matrix((void**)a,MPI_INT,m,n,MPI_COMM_WORLD);

	MPI_Finalize();

}

void compute_this_iteration(int id,int p,int** a,int m,int n,int iter,int print_iter){
	int i,j,k;
	int realRow;
	int rootTop,rootBottom;
	int size;
	int *tmpTopRow;
	int *tmpBottomRow;
	MPI_Status status;

	for(k=0;k<iter;k++){
		int** deA;
		int** resultA;
		int* buff;

		tmpTopRow = (int*)malloc(n*sizeof(int));
		tmpBottomRow = (int*)malloc(n*sizeof(int));
		size = BLOCK_SIZE(id,p,m);
		
		for(i=0;i<p-1;i++){
			if(id==i)
				MPI_Send(a[size-1],n,MPI_INT,i+1,0,MPI_COMM_WORLD);
		}
		for(i=p-1;i>0;i--){
			if(id==i)
				MPI_Send(a[0],n,MPI_INT,i-1,1,MPI_COMM_WORLD);
		}
		for(i=1;i<p;i++){
			if(id==i)
				MPI_Recv(tmpTopRow,n,MPI_INT,i-1,0,MPI_COMM_WORLD,&status);
		}
		for(i=p-2;i>=0;i--){
			if(id==i)
				MPI_Recv(tmpBottomRow,n,MPI_INT,i+1,1,MPI_COMM_WORLD,&status);
		}
		if(id==0)
			for(i=0;i<n;i++) tmpTopRow[i]=0;
		if(id==p-1)
			for(i=0;i<n;i++) tmpBottomRow[i]=0;

		deA = (int**)malloc((size+2)*sizeof(int*));
		
		resultA = (int**)malloc(size*sizeof(int*));
		
		deA[0] = tmpTopRow;
		for(i=0;i<size;i++)
			deA[i+1] = a[i];
		deA[size+1] = tmpBottomRow;
		
		for(i=1;i<size+1;i++){
			buff = (int*)malloc(n*sizeof(int));
			int aliveNum;
			aliveNum = 0;
			for(j=0;j<n;j++){
				if(j==0){
					aliveNum = deA[i-1][j]+deA[i-1][j+1]+deA[i][j+1]+deA[i+1][j+1]+deA[i+1][j];
				}else if(j==n-1){
					aliveNum = deA[i-1][j]+deA[i-1][j-1]+deA[i][j-1]+deA[i+1][j-1]+deA[i+1][j];
				}else{
					aliveNum = deA[i-1][j-1]+deA[i-1][j]+deA[i-1][j+1]+deA[i][j-1]+deA[i][j+1]+deA[i+1][j-1]+deA[i+1][j+1]+deA[i+1][j];
				}
				if(deA[i][j]!=0){
					if(aliveNum<2)
						buff[j]=0;
					else if(aliveNum==2||aliveNum==3)
						buff[j]=1;
					else
						buff[j]=0;
				}else{
					if(aliveNum==3)
						buff[j]=1;
					else
						buff[j]=0;
				}
			}
			resultA[i-1] = (int*)malloc(n*sizeof(int));
			for(j=0;j<n;j++)
				resultA[i-1][j] = buff[j];
			free(buff);
		}
		for(i=0;i<size;i++){
			for(j=0;j<n;j++){
				a[i][j] = resultA[i][j];
			}
		}
		if(k%print_iter==0)
			print_row_striped_matrix((void**)a,MPI_INT,m,n,MPI_COMM_WORLD);

		/*
		if(id==2){
			printf("%d\n",id);
			for(i=0;i<size;i++){
				for(j=0;j<n;j++){
					printf("%d ",a[i][j]);
				}
				printf("\n");
			}
		}
		*/

		/*
		if(id==1){
			for(i=0;i<n;i++){
				printf("%d ", tmpTopRow[i]);
			}
			printf("\n");
			for(i=0;i<n;i++){
				printf("%d ",tmpBottomRow[i]);
			}
		}
		*/
	}
}
