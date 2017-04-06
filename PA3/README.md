# PA3: "Bucket Sort"

## Building and Running
This project includes a makefile which will generate object files and two executable files, one for sequential bucket sort and one for parallel bucket sort.

There are a number of scripts depending on what exectuable you want, how many numbers you want to be sorted, and how many processes you want to participate in the sorting.

For sequential bucket sort, scripts are run like so:

sbatch seq_[1/10/100][K,M,B].sh

In general, you first specify one of three numbers and then a letter that will
represent thousands (K), millions (M), or billions (B). You cannot specify
10 billion or 100 billion numbers.

If you want to view the sorted numbers, add the letter "y" as a command line parameter
to the end of any of the script files themselves. Sorted numbers will be output
to the file "sorted.txt" after the script is run.

If you want to change the number of buckets in the sequential version,
edit the NUM_BUCKETS constant in the "sequential.cpp" file in the /src folder. 

### Makefile Instructions

```bash
cd build
make
sbatch seq_[1/10/100][K,M,B].sh
sbatch par_[1/10/100][K,M,B]_[NUMBER].sh
make clean
```

For example:

Sequential, 10 million items to sort
```bash
sbatch seq_10M.sh
```

Parallel, 10 million items to sort with 16 processes
```bash
sbatch par_10M_16.sh
```

