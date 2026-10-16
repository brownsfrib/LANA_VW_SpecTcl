#ifndef MYPARAMETERMAPPER_H
#define MYPARAMETERMAPPER_H

// C/C++ includes
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
#include <set>
#include <array>
#include <fstream>
#include <cmath>
#include <sstream>

// Project includes
#include <ParameterMapper.h>
#include "parameters.h"

class CEvent;       // SpecTcl event, we don't use it.
//class parameters; // The strucutre of tree parameters.

struct BarPMTs{
	int left 	= -1;
	int right 	= -1;
};

struct ModuleInfo{
	int s_crateID;
	int s_slotID;
	int s_nChannels;

	bool operator<(const ModuleInfo &other) const{
		return std::make_pair(s_crateID, s_slotID) <
			std::make_pair(other.s_crateID, other.s_slotID);
	}
};

//________________________________________________________________________
// Class responsible for mapping DDAS hit data to SpecTcl TreeParameters
//
// It operates on a parameters structure containing TreeParameters but 
// does not own it.
//
class mapper : public DAQ::DDAS::CParameterMapper{
	private:
		parameters&      m_params;  // Reference to the tree parameter structure.
		std::map<int, int> m_chanMap; // Global channel index for crates.
		std::array<BarPMTs, parameters::nBars> m_wallA;
		std::array<BarPMTs, parameters::nBars> m_wallB;

	
	public:
		/**
		* @brief Constructor.
		*
		* @param params The data structure.
		*/    
		mapper(parameters& params, const std::string& inputFile = "config/lanaChannelMapping.txt");
		
		/**
		* @brief Map raw hit data to tree parameters.
		*
		* @param channelData The hit data.
		* @param rEvent      The SpecTcl event.
		*/
		virtual void mapToParameters(
			const std::vector<DAQ::DDAS::DDASHit>& channelData, CEvent& rEvent
		);
		
		/**
		* @brief Compute channel index from crate, slot, and channel information.
		* 
		* @param hit The hit data.
		* 
		* @return The global channel index.
		*/ 
	private:
		void populateWall(
				const std::vector<double>& cfdTimes,
				const std::array<BarPMTs, parameters::nBars>& wall,
				BarData* output);

		void buildChannelMap(std::vector<ModuleInfo> &modules);
		void loadChannelMapping(const std::string& path);
		int computeGlobalIndex(const DAQ::DDAS::DDASHit& hit);
};

#endif

