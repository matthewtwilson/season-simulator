#include <simulator.h>
#include <time.h>
#include <random>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		std::cerr << "usage: " << argv[0] << " infile.txt tie_percent runs" << std::endl;
		exit(0);
	}

	time_t starttime;
	time_t endtime;
	time(&starttime);

	std::ifstream infile;
	infile.open(argv[1]);

	SIMULATOR::NFL::simulator league(infile);
	unsigned int tiePct = std::stoi(argv[2]);
	unsigned long long runs = std::stoi(argv[3]);
	league.processAndRun(runs, tiePct);

	time(&endtime);

	fprintf(stdout,"Ran in %I64d seconds\n",endtime-starttime);

	return 0;
}