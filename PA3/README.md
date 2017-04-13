# PA3: "Bucket Sort"

## Building and Running

This project includes a makefile which will generate object files and two executable files, one for sequential bucket sort and one for parallel bucket sort.

There are a number of scripts depending on what exectuable you want, how many numbers you want to be sorted, and how many processes you want to participate in the sorting.

For sequential bucket sort, scripts are run like so:

sbatch scripts/seq_[1/10/100][K,M,B]_[NUM_BUCKETS].sh

In general, you first specify one of three numbers and then a letter that will
represent thousands (K), millions (M), or billions (B). You cannot specify
10 billion or 100 billion numbers. Then you must specify a number of buckets, which
should be either 2, 4, 8, 12, 16, 24, or 27.

If you want to view the sorted numbers, add the letter "y" as a command line parameter
to the end of any of the script files themselves right before the bucket number.
Sorted numbers will be outputsto the file "sorted.txt" after the script is run. 

For parallel bucket sort, scripts are run like so:

sbatch scripts/parallel_[1/10/100][K,M,B]_[NUM_BUCKETS].sh

In general, you first specify one of three numbers and then a letter that will
represent thousands (K), millions (M), or billions (B). You cannot specify
10 billion or 100 billion numbers. Then you must specify a number of processes, which
should be either 2, 4, 8, 12, 16, 24, or 27.

If you want to view the sorted numbers, add the letter "y" as a command line parameter
to the end of any of the script files themselves right before the process number.
Sorted numbers will be outputsto the file "par_sorted.txt" after the script is run. 

### Makefile Instructions

```bash
cd build
make
sbatch scripts/seq_[1/10/100][K,M,B]_[NUM_BUCKETS].sh
sbatch scripts/parallel_[1/10/100][K,M,B]_[NUM_BUCKETS].sh
make clean
```

For example:

Sequential, 1 million items to sort, 16 buckets:
```bash
sbatch scripts/seq_1M_16.sh
```

Parallel, 1 million items to sort with 16 processes
```bash
sbatch scripts/parallel_1M_16.sh
```

