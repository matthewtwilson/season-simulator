#include "../simulator.h"
#include "../conference.h"
#include "../division.h"
#include "../team.h"
#include "../game.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>

namespace SIMULATOR {
	simulator::simulator(std::ifstream& infile)
		: file(infile)
		, points(0)
		, gen(std::random_device{}()) // seed from a temporary device
	{
	}

	simulator::~simulator()
	{
	}

	void simulator::processAndRun(unsigned long long runs)
	{
		processFile();
		runSimulation(runs);
	}

	team* simulator::createTeam(const std::string& inName)
	{
		return new team(inName, 2);
	}

	division* simulator::createDivision(const std::string& inName)
	{
		return new division(inName, this);
	}

	conference* simulator::createConference(const std::string& inName)
	{
		return new conference(inName);
	}

	void simulator::initializeRecordLabels(unsigned int seasonLength)
	{
		for (int i = 0; i < seasonLength + 1; ++i)
		{
			std::stringstream label;
			label << i << "-" << seasonLength - i;
			recordLabels.push_back(std::make_pair(true, label.str()));
		}
	}

	void simulator::initialOutput()
	{
	}

	unsigned int simulator::calculateRecordBucketFromResults(const std::vector<unsigned int>& real, const std::vector<unsigned int>& proj) const
	{
		return real[0] + proj[0];
	}

	void simulator::processFile()
	{
		enum stage { init, conferencelist, divisionlist, teamlist, preseason, season, postseason };

		stage currStage = init;

		std::string line;
		while (std::getline(file, line))
		{
			//check for state transitions first
			if (line.compare("Conferences") == 0)
				currStage = conferencelist;
			else if (line.compare("Divisions") == 0)
				currStage = divisionlist;
			else if (line.compare("Teams") == 0)
				currStage = teamlist;
			else if (line.compare("Preseason") == 0)
				currStage = preseason;
			else if (line.compare("Season") == 0)
				currStage = season;
			else if (line.compare("Postseason") == 0)
				currStage = postseason;
			else
			{
				switch (currStage)
				{
				case conferencelist:
					processConference(line);
					break;
				case divisionlist:
					processDivision(line);
					break;
				case teamlist:
					processTeam(line);
					break;
				case preseason:
					processPreseasonGame(line);
					break;
				case season:
					processRegularSeasonGame(line);
					break;
				case postseason:
					processPostseasonGame(line);
					break;
				default:
					break;
				}
			}
		}
	}

	void simulator::runSimulation(unsigned long long runs)
	{
		double ppg = static_cast<double>(points) / static_cast<double>(playedGames.size());
		std::cout << "PPG: " << ppg << std::endl;
		std::cout << "Total League Games: " << playedGames.size() + futureGames.size() << std::endl;
		std::cout << "League Games Played: " << playedGames.size() << std::endl;
		std::cout << "Teams: " << teams.size() << std::endl;
		unsigned int seasonLength = ((2 * (playedGames.size() + futureGames.size())) + teams.size() - 1) / teams.size();
		std::cout << "Team Season Length: " << seasonLength << std::endl;

		initializeRecordLabels(seasonLength);
		//Set Team Strengths and record buckets
		for (const auto& it : teams)
		{
			it->initializeRecordBuckets(recordLabels.size());
		}

		//Set Game Probabilities
		for (auto& it : futureGames)
		{
			precalculateProbability(it);
		}
		initialOutput();
		simulateFutureGames(runs);
	}

	void simulator::processConference(std::string& line)
	{
		if (!line.empty())
		{
			conferences.push_back(createConference(line));
		}
	}

	void simulator::processDivision(std::string& line)
	{
		if (!line.empty())
		{
			if (std::string::npos == line.find(',')) //no conference
			{
				divisions.push_back(createDivision(line));
			}
			else
			{
				std::istringstream is(line);
				std::string confName, divName;
				std::getline(is, confName, ',');
				std::getline(is, divName, ',');
				divisions.push_back(createDivision(divName));
				getConference(confName)->addDivision(getDivision(divName));
			}
		}
	}

	void simulator::processTeam(std::string& line)
	{
		if (!line.empty())
		{
			if (std::string::npos == line.find(',')) //no divisions
			{
				teams.push_back(createTeam(line));
			}
			else
			{
				std::istringstream is(line);
				std::string divName, teamName;
				std::getline(is, divName, ',');
				std::getline(is, teamName, ',');
				teams.push_back(createTeam(teamName));
				getDivision(divName)->addTeam(getTeam(teamName));
			}
		}
	}

	//game* convertLineToGame(std::string& line)
	//do this to avoid duplicating logic in processPreseasonGame
	void simulator::processPreseasonGame(std::string& line)
	{
		std::cerr << "unimplemented" << std::endl;
		std::cout << "adding preseason game " << line << std::endl;
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

					std::vector<unsigned int> win = { 1,0 };
					std::vector<unsigned int> loss = { 0,1 };

					if(homePts > awayPts)
					{
						homeTeam->addResult(win);
						awayTeam->addResult(loss);
					}
					else if (awayPts > homePts)
					{
						awayTeam->addResult(win);
						homeTeam->addResult(loss);
					}
					//std::cout << "adding played regular season game " << away << " " << awayPts << " at " << home << " " << homePts << std::endl;
				}
				else
				{
					futureGames.push_back(createGame(homeTeam, awayTeam, 0, 0, false));
					//std::cout << "adding future regular season game " << away << " at " << home << std::endl;
				}
			}
			else
			{
				std::cerr << "One or more invalid team: " << line << std::endl;
			}
		}
	}

	void simulator::processPostseasonGame(std::string& line)
	{
		std::cerr << "unimplemented" << std::endl;
		std::cout << "adding Postseason Game " << line << std::endl;
	}

	game* simulator::createGame(team* home, team* away, const unsigned int home_points, const unsigned int away_points, const bool preseason)
	{
		return new game(home, away, home_points, away_points, preseason);
	}

	void simulator::simulateFutureGames(unsigned long long runs)
	{
		auto checkpoint = runs / 20;
		if (checkpoint < 1)
			checkpoint = 1;
		auto startTime = clock();

		for (auto& r : recordLabels)
		{
			r.first = false;
		}

		for (unsigned long long run = 0; run < runs; ++run)
		{
			if (run % checkpoint == 0)
			{
				std::cerr << run << " runs complete in " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << "seconds." << std::endl;
			}

			for (auto t : teams)
			{
				t->cleanupProjections();
			}

			for (auto g : futureGames)
			{
				simulateOneFutureGame(g);
			}

			for (auto t : teams)
			{
				auto& realResults = t->getResults();
				auto& projResults = t->getProjectedResults();
				auto bucket = calculateRecordBucketFromResults(realResults,projResults);
				t->incrementRecordBucket(bucket);
				recordLabels[bucket].first = true;
			}

		}

		std::cout << "Team,";
		for (auto& r : recordLabels)
		{
			if (r.first)
				std::cout << r.second << ',';
		}
		std::cout << std::endl;

		for (auto& t : teams)
		{
			std::cout << t->getName() << ',';
			for (size_t i = 0; i < recordLabels.size(); ++i)
			{
				if (recordLabels[i].first)
				{
					std::cout << t->getRecordBuckets()[i] << ',';
				}
			}
			std::cout << std::endl;
		}
	}

	void simulator::precalculateProbability(game* g)
	{
		std::vector<double> res = { 0.5, 0.5};
		g->setProbability(res);
	}

	void simulator::simulateOneFutureGame(game* g)
	{
		auto& weights = g->getProbability();
		std::discrete_distribution<> d(weights.begin(), weights.end());
		auto result = d(gen);
		std::vector<unsigned int> win  = { 1,0 };
		std::vector<unsigned int> loss = { 0,1 };
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
		default:
			std::cerr << "simulateOneFutureGame() failure" << std::endl;
			break;
		}

	}

	team* simulator::getTeam(std::string& name) const
	{
		auto lambda = [&name](team* i) { return i->getName() == name; };
		auto retval = std::find_if(teams.begin(), teams.end(), lambda);
		if (retval != std::end(teams))
			return *retval;
		else
			return nullptr;
	}

	division* simulator::getDivision(std::string& name) const
	{
		auto lambda = [&name](division* i) { return i->getName() == name; };
		auto retval = std::find_if(divisions.begin(), divisions.end(), lambda);
		if (retval != std::end(divisions))
			return *retval;
		else
			return nullptr;
	}

	conference* simulator::getConference(std::string& name) const
	{
		auto lambda = [&name](conference* i) { return i->getName() == name; };
		auto retval = std::find_if(conferences.begin(), conferences.end(), lambda);
		if (retval != std::end(conferences))
			return *retval;
		else
			return nullptr;
	}

}

