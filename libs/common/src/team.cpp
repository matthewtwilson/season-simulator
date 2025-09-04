#include "../team.h"
#include <sstream>
#include <cmath>

namespace SIMULATOR {
	team::team(const std::string& inName)
		: name(inName)
		, wins(0)
		, losses(0)
		, ties(0)
		, preseason_points_for(0)
		, preseason_points_against(0)
		, points_for(0)
		, points_against(0)
		, projected_wins(0)
		, projected_losses(0)
		, projected_ties(0)
	{
	}

	team::~team()
	{
	}

	const std::string& team::getName() const
	{
		return name;
	}

	void team::addGame(unsigned int pf, unsigned int pa)
	{
		points_for += pf;
		points_against += pa;
		if (pf > pa)
			++wins;
		else if (pa > pf)
			++losses;
		else
			++ties;
	}

	void team::initializeRecordBuckets(unsigned int sz)
	{
		recordBuckets.resize(sz);
	}


}
