#pragma once
#include <string>
#include <vector>
#include <memory>

/*A conference is a set of divisions 
* in many leagues there are 2 tiers, conferences contain divisions, divisions contain teams.
* In the UFL in 2024, there are only divisions by this definition, even though the UFL calls them conferences. The "XFL Conference" contains 4 teams, and the top 2 make the playoffs
* In most leagues the final game/series is conference champ vs conference champ, but not so in the WNBA from 2016-2021 and 2022-2024.
* Thus, some/much will need to be done in derived classes
*/
namespace SIMULATOR {

	class team;
	class division;

	class conference
	{
	public:
		conference(const std::string& inName);
		~conference();
		const std::string& getName() const;
		void addDivision(SIMULATOR::division* div);
		division* getDivision(std::string divName);
		std::vector<team*> getConferenceTeams() const;
	private:
		std::string name;
		std::vector<division*> divisions;
	};
}