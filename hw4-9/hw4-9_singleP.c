#include <math.h>
#include <stdio.h>
// #include <mpi.h>

double goal = 100;
char marked[100];

int main(int argc, char *argv[]){
	int sqrtGoal = sqrt(goal);
	marked[0] = marked[1] = 1;
	for(int i=2;i<=sqrtGoal;i++){
		if(!marked[i]){
			for(int k=(goal-1)/i,j=i*k;k>=i;k--,j-=i){
				if(!marked[k]) marked[j] = 1;
			}
		}
	}

//	for(int i=2;i<=goal;i++){
//		if(!marked[i]) printf("%d\n",i);
//	}

	int maxGap = 0;
	int prePrime = 2;
	for(int i=3;i<=goal;i++){
		if(!marked[i]){
			int gap = i - prePrime;
			maxGap = (gap>maxGap)?gap:maxGap;
			printf("prePrime: %d; recentPrime: %d; gap: %d; maxGap: %d\n",prePrime, i, gap, maxGap);
			prePrime = i;
		}
	}

	printf("MaxGap: %d\n",maxGap);

	for(int i=0;i<goal;i++){
		printf("%d",marked[i]==1?1:0);
	}

	/* Check the bits. */
	// int size_c = sizeof(char)*256;
	// for(int i=size_c;i;i>>=1) printf(i&n?"1":"0");
}
