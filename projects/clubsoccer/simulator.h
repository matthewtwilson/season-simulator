#pragma once
#include <common/simulator.h>

namespace SIMULATOR {
	namespace CLUBSOCCER {
		class simulator : public SIMULATOR::simulator
		{
		public:
			simulator(std::ifstream& infile);
			~simulator();
			virtual team* createTeam(const std::string& inName) override;
		private:
			virtual void initializeRecordLabels(unsigned int seasonLength) override;
			virtual unsigned int calculateRecordBucketFromResults(const std::vector<unsigned int>& real, const std::vector<unsigned int>& proj) const override;
			virtual void processRegularSeasonGame(std::string& line) override;
			virtual void precalculateProbability(game* g) override;
			virtual void simulateOneFutureGame(game* g);
		};
	}
}