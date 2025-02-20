#pragma once
#include <common/simulator.h>

namespace SIMULATOR {
	namespace NFL {
		class simulator : public SIMULATOR::simulator
		{
		public:
			simulator(std::ifstream& infile);
			~simulator();
		};
	}
}