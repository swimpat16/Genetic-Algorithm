#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

int NUM_FILES = 25;

int main()
{
    for (int i = 0; i < NUM_FILES; i++)
    {
        char command[1000];
        sprintf(command, "./lab5 config2/config%d.cf out/out%d.csv > /dev/null", i, i);
        
        if (system(command) == 2)
        {
            cerr << "Received keyboard interrupt. Terminating execution" << endl;
            exit(1);
        }
        
        printf("Finished config %4d / %d\n", i + 1, NUM_FILES);
    }
}