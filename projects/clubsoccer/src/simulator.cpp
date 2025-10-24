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

		team* simulator::createTeam(const std::string& inName)
		{
			//Soccer has draws, need 3 results
			return new team(inName, 3);
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

		unsigned int simulator::calculateRecordBucketFromResults(const std::vector<unsigned int>& real, const std::vector<unsigned int>& proj) const
		{
			return 3 * real[0] + 3 * proj[0] + real[2] + proj[2];
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

						std::vector<unsigned int> win = { 1,0,0 };
						std::vector<unsigned int> loss = { 0,1,0 };
						std::vector<unsigned int> tie = { 0,0,1 };

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
			auto home_ppg = static_cast<double>(g->getHome()->getPointsFor()) / std::max(1u, g->getHome()->getGamesPlayed());
			auto away_ppg = static_cast<double>(g->getAway()->getPointsFor()) / std::max(1u, g->getAway()->getGamesPlayed());
			auto home_def_ppg = static_cast<double>(g->getHome()->getPointsAgainst()) / std::max(1u, g->getHome()->getGamesPlayed());
			auto away_def_ppg = static_cast<double>(g->getAway()->getPointsAgainst()) / std::max(1u, g->getAway()->getGamesPlayed());
			double league_ppg = static_cast<double>(points) / static_cast<double>(playedGames.size());

			double home_exp = (home_ppg * away_def_ppg) / (league_ppg / 2.0);
			double away_exp = (away_ppg * home_def_ppg) / (league_ppg / 2.0);	

			std::vector<double> res = { home_exp, away_exp };
			g->setProbability(res);
		}

		void simulator::simulateOneFutureGame(game* g)
		{
			std::poisson_distribution<int> home_distribution(g->getProbability()[0]);
			std::poisson_distribution<int> away_distribution(g->getProbability()[1]);
			int home_goals = home_distribution(gen);
			int away_goals = away_distribution(gen);

			std::vector<unsigned int> win = { 1,0,0 };
			std::vector<unsigned int> loss = { 0,1,0 };
			std::vector<unsigned int> tie = { 0,0,1 };
			if (home_goals > away_goals)
			{
				g->getHome()->addProjectedResult(win);
				g->getAway()->addProjectedResult(loss);
			}
			else if (away_goals > home_goals)
			{
				g->getHome()->addProjectedResult(loss);
				g->getAway()->addProjectedResult(win);
			}
			else
			{
				g->getHome()->addProjectedResult(tie);
				g->getAway()->addProjectedResult(tie);
			}
		}

	}
}

