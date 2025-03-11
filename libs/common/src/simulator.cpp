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

	team* simulator::createTeam(const std::string& inName)
	{
		return new team(inName);
	}

	division* simulator::createDivision(const std::string& inName)
	{
		return new division(inName, this);
	}

	conference* simulator::createConference(const std::string& inName)
	{
		return new conference(inName);
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

	void simulator::runSimulation(unsigned long long runs, unsigned int tiePct)
	{
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

