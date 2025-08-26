#pragma once
#include <memory>
#include <vector>

namespace SIMULATOR {
	class team;
	
	class game
	{
	public:
		typedef std::vector<double> result;
		game();
		game(team* inhome, team* inaway, const unsigned int inhome_points, const unsigned int inaway_points, const bool inpreseason);
		~game();
		enum season { undefined, pre, regular, post };
		void setResult(result& r);
		const result& getResult() const;
		void calculateProbability();
		const result& getProbability() const;
		team* getHome() const;
		team* getAway() const;

	private:
		team* home;
		team* away;
		result res;
		result probability;
		season sea;
		unsigned int home_points, away_points;
		bool neutral_site;
		bool preseason;
	};
}