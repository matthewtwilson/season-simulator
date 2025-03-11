#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace SIMULATOR {
/*A division is a set of teams that can't be further subdivided. 
* in many leagues there are 2 tiers, conferences contain divisions, divisions contain teams.
* In the UFL in 2024, there are only divisions by this definition, even though the UFL calls them conferences. The "XFL Conference" contains 4 teams, and the top 2 make the playoffs
* In most leagues the final game/series is conference champ vs conference champ, but not so in the WNBA from 2016-2021 and 2022-2024.
* Thus, some/much will need to be done in derived classes
*/
	//forward declaration
	class simulator;
	class team;

	class division
	{
	public:
		division(const std::string& inName, SIMULATOR::simulator* inSimulator);
		~division();
		const std::string& getName() const;
		void addTeam(team* tm);
		team* getTeam(unsigned int index);
		std::vector<team*> getDivisionTeams() const;
	protected:
		std::vector<team*> teams;
		simulator* sim;
	private:
		std::string name;
	};
}