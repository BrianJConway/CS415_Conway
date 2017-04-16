# set flags and compiler, as well as file var
CC=mpic++
CXXFLAGS= -std=c++11
LIBS=-lpmi
# feel free to either add files here (for more compilcated src's), or create more vars, or just manually type them in. Up to you.
O_FILES1=mpi_sequential.o Timer.o 
O_FILES2=mpi_parallel.o Timer.o
# uncomment below if/when you have a folder called include that holds potential header files
INCLUDES=-I ../include
OUTPUT=mpi_sequential
OUTPUT2=mpi_parallel

all: mpi_sequential mpi_parallel

mpi_sequential: $(O_FILES1)
	$(CC) $(CXXFLAGS) -o $(OUTPUT) $(O_FILES1) $(LIBS) #$(PATHI) $(PATHL)

mpi_parallel: $(O_FILES2)
	$(CC) $(CXXFLAGS) -o $(OUTPUT2) $(O_FILES2) $(LIBS) #$(PATHI) $(PATHL)

mpi_sequential.o: ../src/sequential.cpp
	$(CC) $(CXXFLAGS) -c ../src/sequential.cpp -o mpi_sequential.o $(INCLUDES)

mpi_parallel.o: ../src/parallel.cpp
	$(CC) $(CXXFLAGS) -c ../src/parallel.cpp -o mpi_parallel.o $(INCLUDES)

Timer.o: ../src/Timer.cpp
	$(CC) $(CXXFLAGS) -c ../src/Timer.cpp -o Timer.o $(INCLUDES)

clean:
	rm $(OUTPUT) 
	rm *.o 