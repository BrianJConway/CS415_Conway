# PA3: "Bucket Sort"

## Building and Running
I have included a program for generating uniformly distributed numbers between 0 and 999,999. 
If you follow the instructions below, it will generate an executable called "generator" which
you pass a command line parameter to for specifying how many numbers to generate. Items
will be output to a file called "data.txt" in the build folder.

### Number Generation Instructions

```bash
cd build
g++ -std=c++11 -o generator ../src/generator.cpp
./generator [NUMBER OF ITEMS TO GENERATE]
```

## Building and Running
This project includes a makefile which will generate object files and and executable for sorting numbers using bucket sort.

There is one script for running sequentially. By default, the script "sequential.sh"
will take input from a file named "data.txt" and use 16 buckets to sort. Timing
information will be output to the console.

If you have your numbers in a file other than "data.txt" in the build folder,
simply change the command line parameter in the script "sequential.sh" from "data.txt"
to what your input file is called.

If you want to view the sorted numbers, add the letter "y" as a command line parameter
at the end of the last line of "sequential.sh". Sorted numbers will be output
to the file "sorted.txt" after the script is run.

If you want to change the number of buckets, edi the NUM_BUCKETS constant in the
"sequential.cpp" file in the /src folder. 

### Makefile Instructions

```bash
cd build
make
sbatch sequential.sh
make clean
```
