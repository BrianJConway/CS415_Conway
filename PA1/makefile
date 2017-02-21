# set flags and compiler, as well as file var
CC=mpic++
CXXFLAGS=-Wall
LIBS=-lpmi
# feel free to either add files here (for more compilcated src's), or create more vars, or just manually type them in. Up to you.
O_FILES1=pingPong.o Timer.o 
O_FILES2=bufferChecker.o Timer.o 
# uncomment below if/when you have a folder called include that holds potential header files
INCLUDES=-I../include
OUTPUT=mpi_pingPong
OUTPUT2=mpi_bufferChecker

all: pingPong bufferChecker

pingPong: $(O_FILES1)
	$(CC) $(CXXFLAGS) -o $(OUTPUT) $(O_FILES1) $(LIBS) #$(PATHI) $(PATHL)

bufferChecker: $(O_FILES2)
	$(CC) $(CXXFLAGS) -o $(OUTPUT2) $(O_FILES2) $(LIBS) #$(PATHI) $(PATHL)

pingPong.o: ../src/pingPong.cpp
	$(CC) $(CXXFLAGS) -c ../src/pingPong.cpp -o pingPong.o $(INCLUDES)

bufferChecker.o: ../src/bufferChecker.cpp
	$(CC) $(CXXFLAGS) -c ../src/bufferChecker.cpp -o bufferChecker.o $(INCLUDES)

Timer.o: ../src/Timer.cpp
	$(CC) $(CXXFLAGS) -c ../src/Timer.cpp -o Timer.o $(INCLUDES)

# use this to run
oneBox: $(OUTPUT)
	srun -n2 $(OUTPUT)

twoBox: $(OUTPUT)
	srun -N2 $(OUTPUT)

buffer: $(OUTPUT2)
	srun -N2 $(OUTPUT2)

clean:
	rm $(OUTPUT) 
	rm $(OUTPUT2)
	rm *.o 
    