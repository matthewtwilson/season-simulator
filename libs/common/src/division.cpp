#include "../division.h"
#include <algorithm>
#include "../simulator.h"
#include "../team.h"

namespace SIMULATOR {
		division::division(const std::string& inName, SIMULATOR::simulator* inSimulator)
			: name(inName)
			, sim(inSimulator)
		{
		}

		division::~division()
		{
		}

		const std::string& division::getName() const
		{
			return name;
		}

		void division::addTeam(team* tm)
		{
			teams.push_back(tm);
		}

		team* division::getTeam(unsigned int index)
		{
			if (index < teams.size())
				return teams[index];
			else
				return nullptr;
		}

		std::vector<team*> division::getDivisionTeams() const
		{
			return teams;
		}

}