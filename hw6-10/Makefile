all: MyMPI.o hw6-10.o hw6-10

MyMPI.o: MyMPI.c MyMPI.h
	mpicc -c $< -o $@

hw6-10: hw6-10.o MyMPI.o
	mpicc -g $^ -o $@

hw6-10.o: hw6-10.c
	mpicc -c $< -o $@

clean: 
	rm *.o
	rm hw6-10
