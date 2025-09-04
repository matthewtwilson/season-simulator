#pragma once
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <utility>

namespace SIMULATOR {
	class team;
	class division;
	class conference;
	class game;

	class simulator
	{
	public:
		simulator(std::ifstream& infile);
		~simulator();
		virtual void processAndRun(unsigned long long runs);
	protected:
		virtual team* createTeam(const std::string& inName); //factory method
		virtual division* createDivision(const std::string& inName); //factory method
		virtual conference* createConference(const std::string& inName); //factory method

		std::vector<team*> teams;
		std::vector<conference*> conferences;
		std::vector<division*> divisions;
		std::vector<std::pair<bool, std::string>> recordLabels;
		virtual void initializeRecordLabels(unsigned int seasonLength);
		virtual void initialOutput();
	private:
		void processFile();
		void runSimulation(unsigned long long runs);
		virtual void processConference(std::string& line);
		virtual void processDivision(std::string& line);
		virtual void processTeam(std::string& line);
		virtual void processPreseasonGame(std::string& line);
		virtual void processRegularSeasonGame(std::string& line);
		virtual void processPostseasonGame(std::string& line);
		virtual game* createGame(team* home, team* away, const unsigned int home_points, const unsigned int away_points, const bool preseason); //factory method
		virtual void simulateFutureGames(unsigned long long runs);
		team* getTeam(std::string& name) const;
		division* getDivision(std::string& name) const;
		conference* getConference(std::string& name) const;

		std::ifstream& file;
		std::vector<game*> playedGames;
		std::vector<game*> futureGames;
		unsigned int points;
	};

}