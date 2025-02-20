#include "../simulator.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>

namespace SIMULATOR {
	simulator::simulator(std::ifstream& infile)
		: file(infile)
	{
	}

	simulator::~simulator()
	{
	}

	void simulator::processAndRun(unsigned long long runs, unsigned int tiePct)
	{
		processFile();
		runSimulation(runs, tiePct);
	}

	void simulator::processFile()
	{
	}

	void simulator::runSimulation(unsigned long long runs, unsigned int tiePct)
	{
	}
}

