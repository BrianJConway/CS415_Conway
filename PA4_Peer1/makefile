# Set compiler and libraries to include
CC = mpic++
CXXFLAGS = -Wall
LIBS = -lpmi

# Set source files
PARTSEQ = Sequential-MatrixMultiplication.cpp
PARTDYN = Parallel-MatrixMultiplication.cpp

# Set executable names
EXECUTABLESEQ = Sequential
EXECUTABLEDYN = Parallel

# Make file code
all: $(EXECUTABLEDYN) $(EXECUTABLESEQ)

$(EXECUTABLESEQ): $(PARTSEQ)
	$(CC) $(CXXFLAGS) $(LIBS) -o $(EXECUTABLESEQ) $(PARTSEQ)

$(EXECUTABLEDYN): $(PARTDYN)
	$(CC) $(CXXFLAGS) $(LIBS) -o $(EXECUTABLEDYN) $(PARTDYN)

clean:
	-@if rm $(EXECUTABLESEQ) 2>/dev/null || true; then echo "Cleaned"; else echo "Nothing to clean"; fi
	-@if rm $(EXECUTABLEDYN) 2>/dev/null || true; then echo "Cleaned"; else echo "Nothing to clean"; fi
