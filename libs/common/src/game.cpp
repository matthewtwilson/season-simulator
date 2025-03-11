#include "../game.h"
#include "../team.h"
#include <iostream>

namespace SIMULATOR {
	game::game()
		: home_points(0)
		, away_points(0)
		, preseason(false)
		, homeWinProbability(0.0)
		, res(undefined)
	{
	}

	game::game(team* inhome, team* inaway, const unsigned int inhome_points, const unsigned int inaway_points, const bool inpreseason)
		: home(inhome)
		, away(inaway)
		, home_points(inhome_points)
		, away_points(inaway_points)
		, preseason(inpreseason)
		, homeWinProbability(0.0)
	{
		if (inhome_points > inaway_points)
			setResult(w_home);
		else if (inaway_points > inhome_points)
			setResult(w_away);
		else
			setResult(tied);
	}

	game::~game()
	{
	}

	void game::setResult(result r)
	{
		res = r;
	}

	game::result game::getResult() const
	{
		return res;
	}

	double game::getProbability() const
	{
		return homeWinProbability;
	}

	team* game::getHome() const
	{
		return home;
	}

	team* game::getAway() const
	{
		return away;
	}
}