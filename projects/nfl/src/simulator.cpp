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

		team* simulator::createTeam(const std::string& inName)
		{
			//NFL has ties, need 3 results
			return new team(inName, 3);
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
		}

		unsigned int simulator::calculateRecordBucketFromResults(const std::vector<unsigned int>& real, const std::vector<unsigned int>& proj) const
		{
			return 2*real[0] + 2*proj[0] + real[2] + proj[2];
		}

		void simulator::processRegularSeasonGame(std::string& line)
		{
			std::istringstream is(line);
			int vals = 1 + std::count(line.begin(), line.end(), ',');

			if (2 <= vals)
			{
				std::string away, home;
				std::getline(is, away, ',');
				std::getline(is, home, ',');

				auto homeTeam = getTeam(home);
				auto awayTeam = getTeam(away);
				if (homeTeam && awayTeam)
				{
					if (4 == vals)
					{
						int awayPts = 0, homePts = 0;
						char comma;
						is >> awayPts >> comma >> homePts;
						auto gamePlayed = createGame(homeTeam, awayTeam, homePts, awayPts, false);
						playedGames.push_back(gamePlayed);
						points += homePts;
						points += awayPts;
						homeTeam->addGame(homePts, awayPts);
						awayTeam->addGame(awayPts, homePts);

						std::vector<unsigned int> win  = { 1,0,0 };
						std::vector<unsigned int> loss = { 0,1,0 };
						std::vector<unsigned int> tie  = { 0,0,1 };

						if (homePts > awayPts)
						{
							homeTeam->addResult(win);
							awayTeam->addResult(loss);
						}
						else if (awayPts > homePts)
						{
							awayTeam->addResult(win);
							homeTeam->addResult(loss);
						}
						else
						{
							awayTeam->addResult(tie);
							homeTeam->addResult(tie);
						}
					}
					else
					{
						futureGames.push_back(createGame(homeTeam, awayTeam, 0, 0, false));
					}
				}
				else
				{
					std::cerr << "One or more invalid team: " << line << std::endl;
				}
			}
		}

		void simulator::precalculateProbability(game* g)
		{
			std::vector<double> res = { 0.49, 0.49, 0.02 };
			g->setProbability(res);
		}

		void simulator::simulateOneFutureGame(game* g)
		{
			auto& weights = g->getProbability();
			std::discrete_distribution<> d(weights.begin(), weights.end());
			auto result = d(gen);
			std::vector<unsigned int> win = { 1,0,0 };
			std::vector<unsigned int> loss = { 0,1,0 };
			std::vector<unsigned int> tie = { 0,0,1 };
			switch (result)
			{
			case 0:
				g->getHome()->addProjectedResult(win);
				g->getAway()->addProjectedResult(loss);
				break;
			case 1:
				g->getHome()->addProjectedResult(loss);
				g->getAway()->addProjectedResult(win);
				break;
			case 2:
				g->getHome()->addProjectedResult(tie);
				g->getAway()->addProjectedResult(tie);
				break;
			default:
				std::cerr << "simulateOneFutureGame() failure" << std::endl;
				break;
			}

		}

	}
}

