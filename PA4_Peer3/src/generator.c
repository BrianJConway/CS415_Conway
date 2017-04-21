
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE	10

int main()
{
	// initialization
	unsigned int n;
	unsigned int nIndex, mIndex, pIndex;
	long random_num;
  FILE * outfile;
	outfile = fopen("../bin/dataset.txt", "w");
	// get values for n, m, and p
	printf("n: ");
	scanf("%u",&n);

  if(outfile != NULL)
	{
		// write value of n
    fprintf(outfile, "%u\n", n);
		// create first matrix
    for(nIndex = 0; nIndex < n; nIndex++)
		{
			for(mIndex = 0; mIndex < n; mIndex++)
			{
				random_num = random();
				fprintf(outfile, "%ld\t", (random_num % MAX_SIZE));
			}
			fprintf(outfile, "\n");
    }
		fprintf(outfile, "\n");
		// create second matrix
		for(mIndex = 0; mIndex < n; mIndex++)
		{
			for(pIndex = 0; pIndex < n; pIndex++)
			{
				random_num = random();
				fprintf(outfile, "%ld\t", (random_num % MAX_SIZE));
			}
			fprintf(outfile, "\n");
		}
  }
  fclose(outfile);
  return 0;
}
