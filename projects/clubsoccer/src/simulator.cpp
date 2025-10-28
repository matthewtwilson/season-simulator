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

			// record the simulated goals on the teams (projected) and store match result in the game
			std::vector<unsigned int> win = { 1,0,0 };
			std::vector<unsigned int> loss = { 0,1,0 };
			std::vector<unsigned int> tie = { 0,0,1 };

			// set the game result for head-to-head checks later
			game::result res;
			if (home_goals > away_goals)
				res = { 1.0, 0.0, 0.0 };
			else if (away_goals > home_goals)
				res = { 0.0, 1.0, 0.0 };
			else
				res = { 0.0, 0.0, 1.0 };
			g->setResult(res);

			// update teams' projected goals and results
			g->getHome()->addProjectedGoals(static_cast<unsigned int>(home_goals), static_cast<unsigned int>(away_goals));
			g->getAway()->addProjectedGoals(static_cast<unsigned int>(away_goals), static_cast<unsigned int>(home_goals));

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

		// sortTeams override: identify tied groups and break ties
		void simulator::sortTeams()
		{
			// First sort by primary key (points)
			SIMULATOR::simulator::sortTeams();

			auto key = [this](team* t) -> unsigned int {
				return calculateRecordBucketFromResults(t->getResults(), t->getProjectedResults());
			};

			size_t n = teams.size();
			size_t i = 0;
			while (i < n)
			{
				size_t j = i + 1;
				while (j < n && key(teams[j]) == key(teams[i])) ++j;

				if (j - i > 1)
				{
					// break ties among teams in range [i, j)
					breakTie(teams.begin() + i, teams.begin() + j);
				}

				i = j;
			}
		}

		// helper: compute head-to-head points for team 'a' vs 'b' (played + simulated future games)
		static int h2h_points_from_game(const game* g, const team* a)
		{
			const auto& r = g->getResult();
			if (r.size() >= 3)
			{
				// soccer semantics: r[0] = home win, r[1] = away win, r[2] = tie
				if (g->getHome() == a)
				{
					if (r[0] > r[1] && r[0] > r[2]) return 3;
					if (r[2] > r[0] && r[2] > r[1]) return 1;
					return 0;
				}
				else if (g->getAway() == a)
				{
					if (r[1] > r[0] && r[1] > r[2]) return 3;
					if (r[2] > r[0] && r[2] > r[1]) return 1;
					return 0;
				}
			}
			else if (r.size() == 2)
			{
				// fallback for 2-outcome games (not expected for soccer)
				if (g->getHome() == a)
				{
					if (r[0] > r[1]) return 3;
					return 0;
				}
				else if (g->getAway() == a)
				{
					if (r[1] > r[0]) return 3;
					return 0;
				}
			}
			return 0;
		}

		int headToHeadPoints(const std::vector<game*>& pool, team* a, team* b)
		{
			int pts = 0;
			for (const auto& g : pool)
			{
				if ((g->getHome() == a && g->getAway() == b) || (g->getHome() == b && g->getAway() == a))
				{
					pts += h2h_points_from_game(g, a);
				}
			}
			return pts;
		}

		void simulator::breakTie(std::vector<team*>::iterator first, std::vector<team*>::iterator last)
		{
			const size_t sz = std::distance(first, last);
			if (sz == 2)
			{
				team* a = *first;
				team* b = *(first + 1);

				// compute head-to-head points (played + future)
				int a_pts = headToHeadPoints(playedGames, a, b) + headToHeadPoints(futureGames, a, b);
				int b_pts = headToHeadPoints(playedGames, b, a) + headToHeadPoints(futureGames, b, a);

				if (a_pts != b_pts)
				{
					if (b_pts > a_pts) std::iter_swap(first, first + 1);
					// if a_pts > b_pts, current order (a before b) is already correct
					return;
				}
				// else fall through to next tiebreakers
			}

			// comparator using the remaining tiebreakers:
			// 1) goal differential (total = real + projected)
			// 2) goals for (total = real + projected)
			// 3) wins (real + projected)
			// 4) sum of goal differential and shutouts (gd + real_shutouts + projected_shutouts)
			auto compare = [](team* A, team* B) -> bool
			{
				const unsigned int A_goals_for = A->getPointsFor() + A->getProjectedPointsFor();
				const unsigned int A_goals_against = A->getPointsAgainst() + A->getProjectedPointsAgainst();
				const int A_gd = static_cast<int>(A_goals_for) - static_cast<int>(A_goals_against);

				const unsigned int B_goals_for = B->getPointsFor() + B->getProjectedPointsFor();
				const unsigned int B_goals_against = B->getPointsAgainst() + B->getProjectedPointsAgainst();
				const int B_gd = static_cast<int>(B_goals_for) - static_cast<int>(B_goals_against);

				if (A_gd != B_gd) return A_gd > B_gd;
				if (A_goals_for != B_goals_for) return A_goals_for > B_goals_for;

				unsigned int A_wins = 0;
				unsigned int B_wins = 0;
				if (!A->getResults().empty()) A_wins = A->getResults()[0] + A->getProjectedResults()[0];
				if (!B->getResults().empty()) B_wins = B->getResults()[0] + B->getProjectedResults()[0];
				if (A_wins != B_wins) return A_wins > B_wins;

				int A_final = A_gd + static_cast<int>(A->getRealShutouts() + A->getProjectedShutouts());
				int B_final = B_gd + static_cast<int>(B->getRealShutouts() + B->getProjectedShutouts());
				if (A_final != B_final) return A_final > B_final;

				// deterministic fallback
				return A->getName() < B->getName();
			};

			std::stable_sort(first, last, compare);
		}

	}
}

