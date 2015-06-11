#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
using namespace std;

int NUM_GENES = 0;
int POP_SIZE = 0;
double MUT_PROB = 0.0;
double CROSS_PROB = 0.0;
int R_SEED = 0;
int NUM_GEN = 0;
string outname = "";

vector< vector<bool> > population;
vector<double> fitness;
vector<double> norm_fitness;
vector<double> running_fitness;
vector< vector<double> > statistics;

int loadConstants(int argc, char ** argv)
{
    if (argc == 7)
    {
        // Read input from stdin
        // Genes in the genetic string - l
        NUM_GENES = atoi(argv[1]);
        
        // Population size - N
        POP_SIZE = atoi(argv[2]);
        
        // Mutation probability - Pm
        MUT_PROB = atof(argv[3]);
        
        // Crossover probablily - Pc
        CROSS_PROB = atof(argv[4]);
        
        // Seed for rand - S
        R_SEED = atoi(argv[5]);
        
        // Number of generations - G
        NUM_GEN = atoi(argv[6]);
    }
    else if (argc == 3)
    {
        // Read input from config file if no command line arguments are given
        ifstream infile(argv[1]);
        if (!infile.is_open())
        {
            cerr << "Invalid configuration file name" << endl;
        }
        
        // Genes in the genetic string - l
        infile >> NUM_GENES;
        
        // Population size - N
        infile >> POP_SIZE;
        
        // Mutation probability - Pm
        infile >> MUT_PROB;
        
        // Crossover probablily - Pc
        infile >> CROSS_PROB;
        
        // Seed for rand - S
        infile >> R_SEED;
        
        // Number of generations - G
        infile >> NUM_GEN;
        
        infile.close();
        
        outname = argv[2];
    }
    else
    {
        cerr << "Usage: config_file_name || NUM_GENES POP_SIZE MUT_PROB CROSS_PROB R_SEED NUM_GEN" << endl;
        exit(1);
    }
    
    if(POP_SIZE % 2 == 1)
    {
        cerr << "Please only use an even POP_SIZE" << endl;
        exit(1);
    }
}

void initVectors()
{
    // Initialize Population
    population.resize(POP_SIZE);
    for(int i = 0; i < POP_SIZE; i++)
    {
        population[i].resize(NUM_GENES);
        for(int j = 0; j < NUM_GENES; j++)
        {
            population[i][j] = (rand() % 2 == 1);
        }
    }
    
    // Initialize Statistics
    statistics.resize(NUM_GEN + 1);
    for(int i = 0; i < NUM_GEN + 1; i++)
    {
        statistics[i].resize(3);
        for(int j = 0; j < 3; j++)
        {
            statistics[i][j] = 0.0;
        }
    }
}

long convertToInt(vector<bool> bits)
{
    long sum = 0;
    
    for(int i = 0; i < bits.size(); i++)
    {
        if (bits[i])
            sum += pow(2, i);
    }
    
    return sum;
}

void calculateFitness()
{
    double total_fitness;
    
    fitness.resize(POP_SIZE);
    norm_fitness.resize(POP_SIZE);
    running_fitness.resize(POP_SIZE);
    total_fitness = 0.0;
    
    for(int i = 0; i < POP_SIZE; i++)
    {
        fitness[i] = pow(((double) convertToInt(population[i])) / ((double) pow(2, NUM_GENES)), 10);
        total_fitness += fitness[i];
    }
    
    // Normalize individual fitness values
    for(int i = 0; i < POP_SIZE; i++)
    {
        norm_fitness[i] = fitness[i] / total_fitness;
        if (i == 0)
            running_fitness[i] = norm_fitness[i];
        else
            running_fitness[i] = norm_fitness[i] + running_fitness[i - 1];
    }
}

void calculateStatistics(int statIndex)
{
    int bestIndex;
    
    for(int i = 0; i < POP_SIZE; i++)
    {
        // Average fitness
        statistics[statIndex][0] += fitness[i];
        
        // Fitness of best individual
        if (statistics[statIndex][1] < fitness[i])
        {
            statistics[statIndex][1] = fitness[i];
            bestIndex = i;
        }
    }
    statistics[statIndex][0] /= ((double) POP_SIZE);

    // Number of correct bits in best individual
    statistics[statIndex][2] = 0.0;
    for(int i = 0; i < NUM_GENES; i++)
    {
        if (population[bestIndex][i])
            statistics[statIndex][2] += 1.0;
    }
}

void produceGeneration(int statIndex)
{
    double r1, r2;
    int p1, p2;
    double cross;
    int index;
    double mut;
    vector< vector<bool> > children;
    
    children.resize(POP_SIZE);
    for(int i = 0; i < POP_SIZE; i++)
        children[i].resize(NUM_GENES);
    
    for(int i = 0; i < POP_SIZE / 2; i++)
    {
        // Select two individuals to be parents
        // Get parent 1 index
        r1 = ((double) rand()) / ((double) RAND_MAX);
        p1 = 0;
        while(r1 > running_fitness[p1])
            p1++;
    
        // Get parent 2 index
        p2 = p1;
        while(p1 == p2)
        {
            r2 = ((double) rand()) / ((double) RAND_MAX);
            p2 = 0;
            while(r2 > running_fitness[p2])
                p2++;
        }
        
        // Mate parents and perform any crossover to get offspring
        cross = ((double) rand()) / ((double) RAND_MAX);
        if (cross < CROSS_PROB)
        {
            index = rand() % NUM_GENES;
            
            for(int j = 0; j < NUM_GENES; j++)
            {
                if (j < index)
                {
                    children[i * 2][j] = population[p1][j];
                    children[i * 2 + 1][j] = population[p2][j];
                }
                else
                {
                    children[i * 2][j] = population[p2][j];
                    children[i * 2 + 1][j] = population[p1][j];
                }
            }
        }
        else
        {
            children[i * 2] = population[p1];
            children[i * 2 + 1] = population[p2];
        }
    }
    
    // Perform any mutations on the offspring
    for(int i = 0; i < POP_SIZE; i++)
    {
        for(int j = 0; j < NUM_GENES; j++)
        {
            mut = ((double) rand()) / ((double) RAND_MAX);
            if (mut < MUT_PROB)
                children[i][j] = (!children[i][j]);
        }
    }
    
    // Update the population
    population = children;
    
    // Find population statistics
    calculateFitness();
    calculateStatistics(statIndex);
}

void printStatisticsToCSV()
{
    if (outname == "")
        outname = "out.csv";
    ofstream outfile(outname.c_str(), ofstream::out);
    
    outfile << "Population Size, " << POP_SIZE << "\n";
    outfile << "Number of Generations, " << NUM_GEN << "\n";
    outfile << "Mutation Probability, " << MUT_PROB << "\n";
    outfile << "Crossover Probability, " << CROSS_PROB << "\n";
    outfile << "Number of Genes, " << NUM_GENES << "\n\n";
    
    outfile << "Generation Number, Average Fitness, Best Fitness, Number of Bits\n";
    for (int i = 0; i < NUM_GEN + 1; i++)
        outfile << i << ", " << statistics[i][0] << ", " << statistics[i][1] << ", " << statistics[i][2] << "\n";
    outfile.close();
}

int main(int argc, char ** argv)
{
    loadConstants(argc, argv);
    
    if (R_SEED == -1)
        srand(time(NULL));
    else
        srand(R_SEED);
    
    initVectors();
    calculateFitness();
    calculateStatistics(0);
    
    for(int i = 0; i < NUM_GEN; i++)
    {
        produceGeneration(i + 1);
        cout << "Generation " << i << endl;
    }
        
    printStatisticsToCSV();
}