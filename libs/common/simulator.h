#pragma once
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <utility>
#include <random>

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
		// allow external code (tests) to seed the RNG deterministically
		void seed(unsigned int s);
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
		virtual unsigned int calculateRecordBucketFromResults(const std::vector<unsigned int>& real, const std::vector<unsigned int>& proj) const;
		virtual void processRegularSeasonGame(std::string& line);
		team* getTeam(std::string& name) const;
		virtual game* createGame(team* home, team* away, const unsigned int home_points, const unsigned int away_points, const bool preseason); //factory method
		virtual void precalculateProbability(game* g);
		virtual void simulateOneFutureGame(game* g);
		virtual void sortTeams();

		std::vector<game*> playedGames;
		std::vector<game*> futureGames;
		unsigned int points;
		std::mt19937 gen;
	private:
		void processFile();
		void runSimulation(unsigned long long runs);
		virtual void processConference(std::string& line);
		virtual void processDivision(std::string& line);
		virtual void processTeam(std::string& line);
		virtual void processPreseasonGame(std::string& line);
		virtual void processPostseasonGame(std::string& line);
		virtual void simulateFutureGames(unsigned long long runs);
		division* getDivision(std::string& name) const;
		conference* getConference(std::string& name) const;

		std::ifstream& file;

	};

}