#include <simulator.h>
#include <common/game.h>
#include <common/team.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>

namespace SIMULATOR{
	namespace CLUBSOCCER {
		simulator::simulator(std::ifstream& infile)
			: SIMULATOR::simulator(infile)
		{
		}

		simulator::~simulator()
		{
		}

		void simulator::initializeRecordLabels(unsigned int seasonLength)
		{
			//Wins are 3 points, ties are 1, losses are 0
			//just need 0 to seasonLength*3 buckets
			std::stringstream label;
			for (int i = 0; i < seasonLength*3+1; ++i)
			{
				label.str("");
				label.clear();
				label << i ;
				recordLabels.push_back(std::make_pair(true, label.str()));
			}
		}

	}
}

