#include <simulator.h>
#include <common/game.h>
#include <common/team.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>

namespace SIMULATOR{
	namespace NFL {
		simulator::simulator(std::ifstream& infile)
			: SIMULATOR::simulator(infile)
		{
		}

		simulator::~simulator()
		{
		}

		void simulator::initializeRecordLabels(unsigned int seasonLength)
		{
			//need all records with no ties, plus records with ties
			//(3 wins)   3-0
			//(2.5 wins) 2-0-1
			//(2 wins)   2-1 (1-0-2)
			//(1.5 wins) 1-1-1 (0-0-3)
			//(1 win)    1-2 (0-1-2)
			//(0.5 win)  0-2-1
			//(0 win)    0-3
			std::stringstream label;
			for (int i = 0; i < seasonLength; ++i)
			{
				label.str("");
				label.clear();
				label << i << "-" << seasonLength - i;
				recordLabels.push_back(std::make_pair(true, label.str()));
				label.str("");
				label.clear();
				label << i << "-" << seasonLength - i - 1 << "-1";
				recordLabels.push_back(std::make_pair(true, label.str()));
			}
			label.str("");
			label.clear();
			label << seasonLength << "-0";
			recordLabels.push_back(std::make_pair(true, label.str()));


			recordLabels.push_back(std::make_pair(true, "Division"));
		}

	}
}

