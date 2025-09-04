#pragma once
#include <string>
#include <vector>

namespace SIMULATOR {
	class team
	{
	public:
		team(const std::string& inName);
		virtual ~team();
		const std::string& getName() const;
		void addGame(unsigned int pf, unsigned int pa);
		void initializeRecordBuckets(unsigned int sz);
	protected:
		//for some just index by # of wins, hockey goes by points (2 for win, 1 for OTL currently), soccer is 3 for a win, 1 for a tie
		std::vector<unsigned int> recordBuckets;
		unsigned int wins, losses, ties;
		unsigned int projected_wins, projected_losses, projected_ties;
		unsigned int preseason_points_for, preseason_points_against;
		unsigned int points_for, points_against;
		double strength;
	private:
		std::string name;

	};
}