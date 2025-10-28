#pragma once
#include <string>
#include <vector>

namespace SIMULATOR {
	class team
	{
	public:
		team(const std::string& inName, unsigned int numResults);
		virtual ~team();
		const std::string& getName() const;
		void addGame(unsigned int pf, unsigned int pa);
		void addResult(std::vector<unsigned int>& result);
		void addProjectedResult(std::vector<unsigned int>& result);
		void initializeRecordBuckets(unsigned int sz);
		const std::vector<unsigned int>& getResults() const;
		const std::vector<unsigned int>& getProjectedResults() const;
		virtual void cleanupProjections();
		void incrementRecordBucket(unsigned int x);
		const std::vector<unsigned int>& getRecordBuckets() const;
		const unsigned int getPointsFor() const;
		const unsigned int getPointsAgainst() const;
		const unsigned int getGamesPlayed() const;
		const unsigned int getProjectedGamesPlayed() const;
		void addProjectedGoals(unsigned int pf, unsigned int pa);
		const unsigned int getProjectedPointsFor() const;
		const unsigned int getProjectedPointsAgainst() const;
		const unsigned int getRealShutouts() const;
		const unsigned int getProjectedShutouts() const;

	protected:
		//for some just index by # of wins, hockey goes by points (2 for win, 1 for OTL currently), soccer is 3 for a win, 1 for a tie
		std::vector<unsigned int> recordBuckets;
		std::vector<unsigned int> results;
		std::vector<unsigned int> projected_results;
		unsigned int preseason_points_for, preseason_points_against;
		unsigned int points_for, points_against;

		// projected goal totals and shutouts (added)
		unsigned int projected_points_for, projected_points_against;
		unsigned int real_shutouts, projected_shutouts;

	private:
		std::string name;
		unsigned int num_results;
		unsigned int games_played;
		unsigned int projected_games_played;

	};
}