#pragma once
#include <string>
#include <vector>

namespace SIMULATOR {
	class team
	{
	public:
		team(const std::string& inName);
		~team();
		const std::string& getName() const;
		void addGame(unsigned int pf, unsigned int pa);
	protected:
		unsigned int wins, losses, ties;
		unsigned int projected_wins, projected_losses, projected_ties;
	private:
		std::string name;
		unsigned int preseason_points_for, preseason_points_against;
		unsigned int points_for, points_against;
	};
}