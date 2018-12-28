#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define CHECK_THE_MARKED 0

int main(int argc, char *argv[]){
	int id; /* Process rank */
	int p;  /* Number of processes */
	int n = 1000000; /* 0,1,2,3, ...,n */
	char* marked;
	int findex, lindex, size;
	int* gap; /* gap array */
	int* localGapArray; /* result of return gap, {(dist. betw. first&left),(biggest dist),(dist. betw. last&right])} */
	int prime; /* record the first prime */
	int prePrime; /* record the pre prime */
	int	max_resultGapArray; /* The answer of the problem */
	int tmp_idx;
	int time_iter;

	/* Init */
	MPI_Init(&argc,&argv);
	/* Determine Process Rank */
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	/* Determine Number of Processes */
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	MPI_Barrier(MPI_COMM_WORLD);
	double elapsed_time = -MPI_Wtime();
	for(int i=0;i<1000;i++){
		
		/* Broadcast: method2 */
		/* ex: n = 99 to 4 processors
		   	process 1:  0,  1,  2,  3, ..., 24
			process 2: 25, 26, 27, 28, ..., 49
			process 3: 50, 51, 52, 53, ..., 74
			process 4: 75, 76, 77, 78, ..., 100
		 */
		findex = id*n/p; 
		lindex = (id+1)*n/p-1;
		size   = lindex - findex + 1;
		marked = (char*)malloc((size)*sizeof(char));
	
		/* Initialize the marked */
		for(int i=0;i<size;i++) marked[i]=0;
		/* 2 is prime */
		if(!id) marked[0]=marked[1]=1;
		prime = 2;
		prePrime = prime;
		do{
			/* If findex%prime is 0, then first is 0 */
			/* If findex%prime is 1, then first is prime-1 */
			/* ... */
			int r = findex%prime;
			int first = r==0?0:(prime-r);
			/* We need to reserve the prime, so that if id==0 first+=prime */
			if(!id) first+=(2*prime);
			/* Check the multiple of this prime */
			for(int i=first;i<size;i+=prime) marked[i] = 1;
			// printf("Processer %d - findex: %d / lindex: %d / prime: %d / first: %d\n",id,findex,lindex,prime,first);
			// fflush(stdout);
			if(id==0){
				while(!marked[++prePrime]){
					prime = prePrime;
					prePrime = prime;
				}
			}
			MPI_Bcast(&prime,1,MPI_INT,0,MPI_COMM_WORLD);
			// printf("%d",prime);
		}while(prime*prime<n);
		
		/* build the Gap array */
		gap = (int*)malloc(sizeof(int)*size);
		int gapOf2Prime;
		gapOf2Prime = 0;
		int k;
		k = 0;
		for(int i=0;i<size;i++){
			int maxBig;
			if(marked[i]==1) gapOf2Prime++;
			if(marked[i]==0){
				gap[k++] = ++gapOf2Prime;
				gapOf2Prime = 0;
			}
			if(i==(size-1)) gap[k++] = gapOf2Prime;
		}
		/* Build the local gap array and gather to id=0. */
		localGapArray = (int*)malloc(sizeof(int)*3);
		localGapArray[0] = gap[0];
		int biggestGap = 0;
		for(int i=1;i<k-1;i++)
			biggestGap = gap[i]>biggestGap?gap[i]:biggestGap;
		localGapArray[1] = biggestGap;
		localGapArray[2] = gap[k-1];
		int* glbGapArray = NULL;
		glbGapArray = (int*)malloc(sizeof(int)*p*3);
		if(p>1) MPI_Gather(localGapArray,3,MPI_INT,glbGapArray,3,MPI_INT,0,MPI_COMM_WORLD);
		/* Find Maximum gap */
		int* resultGapArray;
		resultGapArray = (int*)malloc(sizeof(int)*(2*p+1));
		if(!id){
			/*
			for(int i=0;i<3*p;i++){
				printf("%d ",glbGapArray[i]);
			}
			printf("\n");
			*/
			/* Combine the local head and tail num. */
			resultGapArray[0] = glbGapArray[0];
			tmp_idx = 1;
			for(int i=1;i<3*p;i+=3,tmp_idx+=2)
				resultGapArray[tmp_idx] = glbGapArray[i];
			tmp_idx = 2;
			for(int i=2;i<3*p-1;i+=3,tmp_idx+=2)
				resultGapArray[tmp_idx] = glbGapArray[i]+glbGapArray[i+1];
			resultGapArray[2*p] = glbGapArray[3*p-1];
			/*
			for(int i=0;i<2*p+1;i++)
				printf("%d ",resultGapArray[i]);
			printf("\n");
			*/
			max_resultGapArray = 0;
			for(int i=0;i<2*p+1;i++)
				max_resultGapArray = resultGapArray[i]>max_resultGapArray?resultGapArray[i]:max_resultGapArray;
	
		}
	}
	if(!id) printf("The largest gap between 1 ~ %d: %d\n",n,max_resultGapArray-1);
	elapsed_time += MPI_Wtime();
	if(!id) printf("Execution time of Process %d: %10.6f\n",id,elapsed_time/1000);

	/* print marked */
	if(CHECK_THE_MARKED){
		for(int i;i<p;i++){
			/* delay time */
			for(int i=0;i<10000;i++)
				for(int j=0;j<10000;j++)
					time_iter++;
			if(i==id){
				printf("Process: %d / findex: %d / lindex: %d\n",id,findex,lindex);
				printf("Prime label: ");
				for(int j=0;j<size;j++)
					printf("%d ",marked[j]==0?0:1);
				printf("\n");
				printf("Prime gap:   ");
				for(int j=0;j<size;j++)
					printf("%d ",gap[j]);
				printf("\n");
				printf("Result gap:  ");
				for(int i=0;i<3;i++)
					printf("%d ",localGapArray[i]);
				printf("\n\n");
			}
		}
	}
	
	/* Shut down MPI */
	MPI_Finalize();
}
