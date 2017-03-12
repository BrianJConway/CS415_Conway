# set flags and compiler, as well as file var
CC=mpic++
CXXFLAGS=-Wall
LIBS=-lpmi
# feel free to either add files here (for more compilcated src's), or create more vars, or just manually type them in. Up to you.
O_FILES1=mpi_sequential.o Timer.o PIMFuncs.o
O_FILES2=mpi_static.o Timer.o PIMFuncs.o
# uncomment below if/when you have a folder called include that holds potential header files
INCLUDES=-I ../include
OUTPUT=mpi_sequential
OUTPUT2=mpi_static

all: mpi_sequential mpi_static

mpi_sequential: $(O_FILES1)
	$(CC) $(CXXFLAGS) -o $(OUTPUT) $(O_FILES1) $(LIBS) #$(PATHI) $(PATHL)

mpi_static: $(O_FILES2)
	$(CC) $(CXXFLAGS) -o $(OUTPUT2) $(O_FILES2) $(LIBS) #$(PATHI) $(PATHL)

mpi_sequential.o: ../src/sequential.cpp
	$(CC) $(CXXFLAGS) -c ../src/sequential.cpp -o mpi_sequential.o $(INCLUDES)

mpi_static.o: ../src/static.cpp
	$(CC) $(CXXFLAGS) -c src/static.cpp -o mpi_static.o $(INCLUDES)

Timer.o: ../src/Timer.cpp
	$(CC) $(CXXFLAGS) -c ../src/Timer.cpp -o Timer.o $(INCLUDES)

PIMFuncs.o: ../src/PIMFuncs.cpp
	$(CC) $(CXXFLAGS) -c ../src/PIMFuncs.cpp -o PIMFuncs.o $(INCLUDES)

# use this to run
sequential: $(OUTPUT)
	srun -n1 $(OUTPUT)

static: $(OUTPUT)
	srun -N2 $(OUTPUT)

#buffer: $(OUTPUT2)
#	srun -N2 $(OUTPUT2)

clean:
	rm $(OUTPUT) 
#	rm $(OUTPUT2)
	rm *.o 
	rm *.pim    