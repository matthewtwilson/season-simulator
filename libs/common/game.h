#pragma once
#include <memory>

namespace SIMULATOR {
  class team;
  
	class game
	{
	public:
		game();
		game(team* inhome, team* inaway, const unsigned int inhome_points, const unsigned int inaway_points, const bool inpreseason);
		~game();
		enum result { undefined, w_home, w_away, tied };
		void setResult(result r);
		result getResult() const;
		double getProbability() const;
		team* getHome() const;
		team* getAway() const;

	private:
		team* home;
		team* away;
		result res;
		unsigned int home_points, away_points;
		bool preseason;
		double homeWinProbability;
	};
}