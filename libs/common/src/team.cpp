#include "../team.h"
#include <sstream>
#include <cmath>
#include <cassert>

namespace SIMULATOR {
	team::team(const std::string& inName, unsigned int numResults)
		: name(inName)
		, preseason_points_for(0)
		, preseason_points_against(0)
		, points_for(0)
		, points_against(0)
		, games_played(0)
		, projected_games_played(0)
		, num_results(numResults)
	{
		results.resize(num_results, 0);
		projected_results.resize(num_results, 0);
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
	}

	void team::addResult(std::vector<unsigned int>& result)
	{
		if (result.size() != results.size())
		{
			throw std::runtime_error("team::addResult vectors not same size");
		}

		for (int i = 0; i < result.size(); ++i)
		{
			results[i] += result[i];
		}

		++games_played;
	}

	void team::addProjectedResult(std::vector<unsigned int>& result)
	{
		if (result.size() != projected_results.size())
		{
			throw std::runtime_error("team::addProjectedResult vectors not same size");
		}

		for (int i = 0; i < result.size(); ++i)
		{
			projected_results[i] += result[i];
		}

		++projected_games_played;
	}

	void team::initializeRecordBuckets(unsigned int sz)
	{
		recordBuckets.resize(sz,0);
	}

	const std::vector<unsigned int>& team::getResults() const
	{
		return results;
	}

	const std::vector<unsigned int>& team::getProjectedResults() const
	{
		return projected_results;
	}

	void team::cleanupProjections()
	{
		std::fill(projected_results.begin(), projected_results.end(), 0);
		projected_games_played = 0;
	}

	void team::incrementRecordBucket(unsigned int x)
	{
		assert(x < recordBuckets.size());
		++recordBuckets[x];
	}

	const std::vector<unsigned int>& team::getRecordBuckets() const
	{
		return recordBuckets;
	}

	const unsigned int team::getPointsFor() const
	{
		return points_for;
	}

	const unsigned int team::getPointsAgainst() const
	{
		return points_against;
	}

	const unsigned int team::getGamesPlayed() const
	{
		return games_played;
	}

	const unsigned int team::getProjectedGamesPlayed() const
	{
		return projected_games_played;
	}


}
