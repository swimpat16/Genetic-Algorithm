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

int main()
{
    int count = 0;
    
    string genes[] = {"10", "15", "20", "25", "30"};
    string pop_size[] = {"10", "30", "50", "100", "1000"};
    string generations[] = {"10", "25", "50", "100", "1000"};
    string mut_prob[] = {"0.01", "0.03", "0.05", "0.1", "0.2"};
    string cross_prob[] = {"0.25", "0.5", "0.6", "0.7", "0.8"};
    
    for(int i = 0; i < 5; i++)
    {
        //Open and name config file
        char filename[100];
        sprintf(filename, "config2/config%d.cf", count++);
        ofstream outfile(filename, ofstream::out);
    
        //Make sure config file opened
        if (!outfile.is_open())
        {
            cerr << "Cannot open " << filename << endl;
            exit(1);
        }
        
        // genes, pop, mut, cross, seed, generations
        // Print Number of genes
        outfile << genes[i] << endl;
        
        // Print population size
        outfile << pop_size[2] << endl;
        
        // Print mutation probability
        outfile << mut_prob[2] << endl;
        
        // Print cross probability
        outfile << cross_prob[3] << endl;
        
        // Print random seed
        outfile << "-1" << endl;
        
        // Print number of generations
        outfile << generations[10] << endl;
        
        
        // Close outfile
        outfile.close();
        
    }
    
   
    return 0;
}