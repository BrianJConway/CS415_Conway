# set flags and compiler, as well as file var
CC=mpic++
CXXFLAGS=-Wall -std=c++11 $(DEBUG)
DEBUG=-g
GPROF=-pg
LIBS=-lpmi
FILEDATA=../src/generator.c
FILEDATANONSQUARE=../src/generator_nonsquare.c
FILESEQ=../src/sequential_matrixmultiplication.cpp
FILENONSQUARE=../src/sequential_matrixmultiplication_nonsquare.cpp
FILEPAR=../src/parallel_matrixmultiplication.cpp
FILEFUNCS=../src/matrix_funcs.cpp
FILEFUNCSNONSQUARE=../src/matrix_funcs_nonsquare.cpp

OUTPUTDATA=generator
OUTPUTDATANONSQUARE=generator_nonsquare
OUTPUTSEQ=sequential_matrixmultiplication
OUTPUTNONSQUARE=sequential_matrixmultiplication_nonsquare
OUTPUTPAR=parallel_matrixmultiplication
OUTPUTSLURM=*.out

OBJFUNCS=matrix_funcs.o
OBJFUNCSNONSQUARE=matrix_funcs_nonsquare.o

all: $(OUTPUTDATA) $(OUTPUTDATANONSQUARE) $(OUTPUTSEQ) $(OUTPUTNONSQUARE) $(OUTPUTPAR)

$(OUTPUTDATA): $(FILEDATA)
	gcc $(FILEDATA) -o $(OUTPUTDATA)

$(OUTPUTDATANONSQUARE): $(FILEDATANONSQUARE)
	gcc $(FILEDATANONSQUARE) -o $(OUTPUTDATANONSQUARE)

$(OUTPUTSEQ): $(FILESEQ) $(OBJFUNCS)
	$(CC) $(CXXFLAGS) $(FILESEQ) $(OBJFUNCS) -o $(OUTPUTSEQ)

$(OUTPUTNONSQUARE): $(FILENONSQUARE) $(OBJFUNCSNONSQUARE)
	$(CC) $(CXXFLAGS) $(FILENONSQUARE) $(OBJFUNCSNONSQUARE) -o $(OUTPUTNONSQUARE)

$(OUTPUTPAR): $(FILEPAR) $(OBJFUNCS)
	$(CC) $(CXXFLAGS) $(FILEPAR) $(OBJFUNCS) -o $(OUTPUTPAR) $(LIBS)

$(OBJFUNCS): $(FILEFUNCS)
	$(CC) $(CXXFLAGS) -c $(FILEFUNCS) $(LIBS)

$(OBJFUNCSNONSQUARE): $(FILEFUNCSNONSQUARE)
	$(CC) $(CXXFLAGS) -c $(FILEFUNCSNONSQUARE) $(LIBS)

clean:
	-@if rm $(OUTPUTSEQ) $(OUTPUTNONSQUARE) $(OUTPUTDATA) $(OUTPUTDATANONSQUARE) $(OUTPUTPAR) $(OBJFUNCS) $(OBJFUNCSNONSQUARE) $(OUTPUTSLURM) 2>/dev/null || true; then echo "Cleaned"; else echo "Nothing to clean"; fi
