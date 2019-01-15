#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define Block_low(id ,p,n) ((id)*(n)/(p))
#define Block_high(id,p,n) (Block_low((id)+1,p,n)-1)
#define Block_size(id,p,n) (Block_high(id,p,n)-Block_low(id,p,n)+1)



int main(int argc, char *argv[]) {
	int n = 900000;
	double time, timetotal;
	int numproc, id, size, lowvalue, highvalue;
	int count = 0, counttotal=0;
	int sum=0;
	int numstart = 100000;
	int digits = 6;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	size = Block_size(id, numproc, n);
	//printf("blocksize:%d\n",size);

	int **array = (int**)malloc(sizeof(int*)*n / numproc);
	for (int i = 0; i != n / numproc; i++) {
		array[i] = (int*)malloc(sizeof(int) * 6); //•ãšµdata”³’ù”ì’ö—ú§F””šmŸ7ž 
	}

	lowvalue = numstart + Block_low(id, numproc, n);
	//printf("lowvalue:%d\n",lowvalue);

	highvalue = numstart + Block_high(id, numproc, n);
	//printf("highvalue:%d\n",highvalue);

	MPI_Barrier(MPI_COMM_WORLD);
	time = -MPI_Wtime();

	//int w;
	for (int i =lowvalue;i<highvalue+1;i++)
	{
		int one = i / 100000;
		int two = i / 10000 - (i / 100000) * 10;
		int three = (i / 1000) - one * 100 - two * 10;
		int four = (i / 100) - one * 1000 - two * 100-three*10;
		int five = (i / 10) - one * 10000 - two * 1000 - three * 100-four*10;
		int six = i % 10;
		array[i - lowvalue][0] = one;
		array[i - lowvalue][1] = two;
		array[i - lowvalue][2] = three;
		array[i - lowvalue][3] = four;
		array[i - lowvalue][4] = five;
		array[i - lowvalue][5] = six;
		for (int j = 0; j < digits; j++)
		{
			//sum += array[i - lowvalue][j];
			if (array[i - lowvalue][j] == array[i - lowvalue][j + 1])
			{
				count++;
				break;
			}
			sum += array[i - lowvalue][j];
		}

		if (sum == 7 || sum == 11 || sum == 13)
		{
			count++;
		}
	}

	MPI_Reduce(&count, &counttotal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (id == 0)printf("%d\n", n - counttotal);

	for (int i = 0; i != n / numproc; i++)
		free(array[i]);
	free(array);

	MPI_Barrier(MPI_COMM_WORLD);
	time += MPI_Wtime();
	MPI_Reduce(&time, &timetotal, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (id == 0)printf("time is :%f\n", timetotal);

	MPI_Finalize();
	return 0;
}
