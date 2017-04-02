#include <cstdlib>
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

const int MIN_NUM = 0;
const int MAX_NUM = 999999;

int main(int argc, char **argv)
{
    // Initialization
    int index, numItems;
    ofstream fout;
    random_device rd;
    default_random_engine generator( rd() );
    uniform_int_distribution<int> dist1( MIN_NUM, MAX_NUM );

    // Check for command line arguments
    if (argc >= 2)
    {
        // Get number of items to generate
        numItems = atoi(argv[1]);

        // Open output file
        fout.open("../build/data.txt");

        // Output number of items
        fout << numItems << endl;

        // Loop through all numbers to generate
        for(int index = 0; index < numItems; index++)
        {
            // Generate current number and output to file
            fout << dist1(generator) << endl;
        }
        // end Loop

        // Close output file
        fout.close();
    }
}