# set flags and compiler, as well as file var
CC=mpic++
CXXFLAGS= -std=c++11
LIBS=-lpmi
# feel free to either add files here (for more compilcated src's), or create more vars, or just manually type them in. Up to you.
O_FILES1=mpi_sequential.o Timer.o 
O_FILES2=mpi_parallel.o Timer.o
O_FILES3=mpi_parallel2.o Timer.o
# uncomment below if/when you have a folder called include that holds potential header files
INCLUDES=-I ../include
OUTPUT=mpi_sequential
OUTPUT2=mpi_parallel
OUTPUT3=mpi_parallel_file


all: mpi_sequential mpi_parallel mpi_parallel_file

mpi_sequential: $(O_FILES1)
	$(CC) $(CXXFLAGS) -o $(OUTPUT) $(O_FILES1) $(LIBS) #$(PATHI) $(PATHL)

mpi_parallel: $(O_FILES2)
	$(CC) $(CXXFLAGS) -o $(OUTPUT2) $(O_FILES2) $(LIBS) #$(PATHI) $(PATHL)

mpi_parallel_file: $(O_FILES3)
	$(CC) $(CXXFLAGS) -o $(OUTPUT3) $(O_FILES3) $(LIBS) #$(PATHI) $(PATHL)

mpi_sequential.o: ../src/sequential.cpp
	$(CC) $(CXXFLAGS) -c ../src/sequential.cpp -o mpi_sequential.o $(INCLUDES)

mpi_parallel.o: ../src/parallel_generated.cpp
	$(CC) $(CXXFLAGS) -c ../src/parallel_generated.cpp -o mpi_parallel.o $(INCLUDES)

mpi_parallel2.o: ../src/parallel_file.cpp
	$(CC) $(CXXFLAGS) -c ../src/parallel_file.cpp -o mpi_parallel2.o $(INCLUDES)

Timer.o: ../src/Timer.cpp
	$(CC) $(CXXFLAGS) -c ../src/Timer.cpp -o Timer.o $(INCLUDES)

clean:
	rm $(OUTPUT) 
	rm $(OUTPUT2) 
	rm $(OUTPUT3) 
	rm *.o 