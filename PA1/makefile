# set flags and compiler, as well as file var
CC=mpic++
CXXFLAGS=-Wall
LIBS=-lpmi
# feel free to either add files here (for more compilcated src's), or create more vars, or just manually type them in. Up to you.
O_FILES=main.o Timer.o
FILE=../src/main.cpp
# uncomment below if/when you have a folder called include that holds potential header files
INCLUDES=-I../include
OUTPUT=mpi_pingpong

all: $(O_FILES)
	$(CC) $(CXXFLAGS) -o $(OUTPUT) $(O_FILES) $(LIBS) #$(PATHI) $(PATHL)
    

main.o: ../src/main.cpp
	$(CC) $(CXXFLAGS) -c ../src/main.cpp -o main.o $(INCLUDES)

Timer.o: ../src/Timer.cpp
	$(CC) $(CXXFLAGS) -c ../src/Timer.cpp -o Timer.o $(INCLUDES)

# use this to run
oneBox: $(OUTPUT)
	srun -n2 $(OUTPUT)

twoBox: $(OUTPUT)
	srun -N2 $(OUTPUT)

clean:
	-@if rm $(OUTPUT) 2>/dev/null || true; then echo "Cleaned"; else echo "Nothing to clean"; fi
