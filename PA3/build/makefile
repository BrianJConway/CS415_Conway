# set flags and compiler, as well as file var
CC=mpic++
CXXFLAGS=
LIBS=-lpmi
# feel free to either add files here (for more compilcated src's), or create more vars, or just manually type them in. Up to you.
O_FILES1=mpi_sequential.o Timer.o 
# uncomment below if/when you have a folder called include that holds potential header files
INCLUDES=-I ../include
OUTPUT=mpi_sequential

all: mpi_sequential 

mpi_sequential: $(O_FILES1)
	$(CC) $(CXXFLAGS) -o $(OUTPUT) $(O_FILES1) $(LIBS) #$(PATHI) $(PATHL)

mpi_sequential.o: ../src/sequential.cpp
	$(CC) $(CXXFLAGS) -c ../src/sequential.cpp -o mpi_sequential.o $(INCLUDES)

Timer.o: ../src/Timer.cpp
	$(CC) $(CXXFLAGS) -c ../src/Timer.cpp -o Timer.o $(INCLUDES)

# use this to run
sequential: $(OUTPUT)
	srun -n1 $(OUTPUT)

clean:
	rm $(OUTPUT) 
	rm *.o 
	rm *.pim    