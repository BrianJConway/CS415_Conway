# set flags and compiler, as well as file var
CC=mpic++
CXXFLAGS=-Wall
LIBS=-lpmi
# feel free to either add files here (for more compilcated src's), or create more vars, or just manually type them in. Up to you.
FILE=../src/main.cpp
FILE=../src/Timer.cpp
# uncomment below if/when you have a folder called include that holds potential header files
INCLUDES=-I../include
OUTPUT=mpi_pingpong

all: $(OUTPUT)

$(OUTPUT): $(FILE)
	$(CC) $(CXXFLAGS) -o $(OUTPUT) $(FILE) $(LIBS)

# use this to run
run: $(OUTPUT)
	srun -n16 $(OUTPUT)

clean:
	-@if rm $(OUTPUT) 2>/dev/null || true; then echo "Cleaned"; else echo "Nothing to clean"; fi
