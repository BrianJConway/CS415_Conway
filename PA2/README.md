# PA2: "Mandelbrot"

# Building and Running

## Building and Running
This project includes a makefile which will generate object files and two executables, one for calculating mandelbrot sequentially and one calculating
mandelbrot in parallel with static task allocation. 

There is one script for running sequentially.

There are many scripts for running static in parallel depending on number of
tasks you want to specify. The static scripts are formatted as 
"static" and then an odd number for the amount of tasks, then ".sh".

Image resolution can be adjusted by editing constants in either
static.cpp or sequential.cpp in the /src folder

The sequential image is output to "imageSeq.pim" and the parallel image is
output to "imageStat.pim". The time taken to calculate all the pixel colors
is output to the console.

### Makefile Instructions

```bash
cd build
make
sbatch sequential.sh
sbatch static[ODD NUMBER].sh
make clean
```
