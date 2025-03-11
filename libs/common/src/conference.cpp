#include "../conference.h"
#include "../team.h"
#include "../division.h"
#include <algorithm>

namespace SIMULATOR {

	conference::conference(const std::string& inName)
		: name(inName)
	{
	}

	conference::~conference()
	{
	}

	const std::string& conference::getName() const
	{
		return name;
	}

	void conference::addDivision(division* div)
	{
		divisions.push_back(div);
	}

	division* conference::getDivision(std::string divName)
	{
		auto retval = std::find_if(divisions.begin(), divisions.end(), [&divName](division* d) {return d->getName() == divName; });
		if (retval == divisions.end())
			return nullptr;
		else
			return *retval;
	}

	std::vector<team*> conference::getConferenceTeams() const
	{
		std::vector<team*> retval;

		for (auto divIt : divisions)
		{
			for (auto teamIt : divIt->getDivisionTeams())
			{
				retval.push_back(teamIt);
			}
		}

		return retval;
	}

}
