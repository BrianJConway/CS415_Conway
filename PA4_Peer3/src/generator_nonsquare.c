
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE	10

int main()
{
	// initialization
	unsigned int n, m, p;
	unsigned int nIndex, mIndex, pIndex;
	long random_num;
  FILE * outfile;
	outfile = fopen("../bin/dataset_nonsquare.txt", "w");
	// get values for n, m, and p
	printf("n: ");
	scanf("%u",&n);
	printf("m: ");
	scanf("%u",&m);
	printf("p: ");
	scanf("%u",&p);
  if(outfile != NULL)
	{
		// write values of n, m, p
    fprintf(outfile, "%u %u %u\n", n, m, p);
		// create first matrix
    for(nIndex = 0; nIndex < n; nIndex++)
		{
			for(mIndex = 0; mIndex < m; mIndex++)
			{
				random_num = random();
				fprintf(outfile, "%ld\t", (random_num % MAX_SIZE));
			}
			fprintf(outfile, "\n");
    }
		fprintf(outfile, "\n");
		// create second matrix
		for(mIndex = 0; mIndex < m; mIndex++)
		{
			for(pIndex = 0; pIndex < p; pIndex++)
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
