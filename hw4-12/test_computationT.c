#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n 1000000000

double f(int i){
	double x;
	x = (double)i/(double)n;
	return 4.0/(1.0+x*x);
}

int main(int argc, char* argv[]){
	double area;
	int i;
	clock_t start_time,end_time;
	float total_time;
	start_time = clock();

	for(i=0;i<n;i++)
		area+=4.0*f(2*i-1)+2*f(2*i);
	area/=(3.0*n);

	end_time = clock();
	total_time = (float)(end_time-start_time)/CLOCKS_PER_SEC;
	printf("n                  : %d\n"     ,n             );
	printf("Execution time     : %13.11f\n",total_time    );
	return 0;
}
