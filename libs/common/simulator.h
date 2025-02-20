#pragma once
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <utility>

namespace SIMULATOR {
	class simulator
	{
	public:
		simulator(std::ifstream& infile);
		~simulator();
		virtual void processAndRun(unsigned long long runs, unsigned int tiePct);
	private:
		void processFile();
		void runSimulation(unsigned long long runs, unsigned int tiePct);

		std::ifstream& file;
	};

}