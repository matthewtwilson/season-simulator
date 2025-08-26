#include "../game.h"
#include "../team.h"
#include <iostream>

namespace SIMULATOR {
	game::game()
		: home_points(0)
		, away_points(0)
		, preseason(false)
		, res(undefined)
	{
	}

	game::game(team* inhome, team* inaway, const unsigned int inhome_points, const unsigned int inaway_points, const bool inpreseason)
		: home(inhome)
		, away(inaway)
		, home_points(inhome_points)
		, away_points(inaway_points)
		, preseason(inpreseason)
	{
		game::result end;
		if (inhome_points > inaway_points)
			end = { 1.0,0.0,0.0 };
		else if (inaway_points > inhome_points)
			end = { 0.0,1.0,0.0 };
		else
			end = { 0.0,0.0,1.0 };
		setResult(end);
	}

	game::~game()
	{
	}

	void game::setResult(result& r)
	{
		res = r;
	}

	const game::result& game::getResult() const
	{
		return res;
	}

	void game::calculateProbability()
	{
		probability = { 0.5,0.5,0.0 };
	}

	const game::result& game::getProbability() const
	{
		return probability;
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