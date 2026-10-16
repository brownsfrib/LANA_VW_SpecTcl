// Project includes
#include "mapper.h"
#include "parameters.h"

// Make it possible to write DDASHit instead of DAQ::DDAS::DDASHit:
using DAQ::DDAS::DDASHit;

//________________________________________________________________________
// Initialize the parameters and channel map.
//
// To add additional crates:
//     m_chanMap[crateID] = startIndex;
//
// A full crate of Pixie-16 digitizers contains 208 channels. For a second
// crate, starting at channel 208, one would add the fllowing line:
//     m_chanMap[1] = 208;
//
mapper::mapper(parameters& params, const std::string& mappingFile) : m_params(params), m_chanMap(){
	loadChannelMapping(mappingFile);
}

void mapper::loadChannelMapping(const std::string& path){
	std::ifstream fileIn(path);
	if (!fileIn){
		std::cerr << "\033[1;31mmapper::loadChannelMapping -- could not open channel map '"
			<< path << "'. Summary plots will not be filled.\033[0m\n";
		return;
	}
	
	std::string line;	
	std::getline(fileIn, line);
	int lineNo = 1; // skipping header
	int nLoaded = 0;
	while (std::getline(fileIn, line)){
		lineNo++;
		if (line.find_first_not_of(" \t\r") == std::string::npos){
			continue;
		}

		// i fucking hate parsing in C++ this shit sucks
		// it would almost be better to use fucking root to do this jesus
		std::istringstream ss(line);
		int pixie_module;
		int channel;
		int bar;
		std::string wall, side;
		if (!(ss >> pixie_module >> channel >> wall >> bar >> side)){
			std::cerr << "\033[1;31mmapper::loadChannelMapping -- line " << 
				lineNo <<
				" : cannot be parsed, skipped\033[0m\n";
			continue;
		}
		if (
			(
			 (wall != "A")
			 &&(wall != "B")
			) ||
			(
			 (side != "L")
			 &&(side != "R")
			)
		){
			std::cerr << "\033[1;31mmapper::loadChannelMapping -- line " << 
				lineNo << 
				" : can't be parsed, skipping\033[0m\n";
			continue;
		}
		if (
			(pixie_module < 0) 
			||(pixie_module >= parameters::nModules)
			||(channel < 0)
			||(channel >= parameters::nChannels)
			||(bar < 0)
			||(bar >= parameters::nBars)
		){
			std::cerr << "\033[1;31mmapper::loadChannelMapping -- line " <<
				lineNo <<
				" : out of range, skipping\033[0m\n";
			continue;
		}

		// we could've rewritten computeGlobalIdx but it's fine
		int globalChannelIdx = pixie_module*parameters::nChannels + channel;
		BarPMTs& pmts = (wall == "A") ? m_wallA[bar] : m_wallB[bar];
		if (side == "L"){
			pmts.left = globalChannelIdx;
		}
		else{
			pmts.right = globalChannelIdx;
		}
		nLoaded++;
	}
	std::cout << "\033[2;32mLANA mapping: loaded " << nLoaded << " PMTs from " << path << "\033[0m\n";
}

void mapper::populateWall(
		const std::vector<double>& cfdTimes, 
		const std::array<BarPMTs, parameters::nBars>& wall, 
		BarData* output){

	for (int bIdx=0; bIdx < parameters::nBars; bIdx++){
		const BarPMTs& pmt = wall[bIdx];
		if ((pmt.left < 0) || (pmt.right < 0)){ // this would happen, for example, if you dropped a ladder on nwa
			// like i did
			// anyway

			// let us
			continue;
		}
		if ((!std::isnan(cfdTimes[pmt.left])) || (!std::isnan(cfdTimes[pmt.right]))){
			output[bIdx].timeDiff = cfdTimes[pmt.right] - cfdTimes[pmt.left];
		}
	}	
}

//________________________________________________________________________
// 
void mapper::mapToParameters(
		const std::vector<DDASHit>& channelData,
		CEvent& rEvent){

	const int nTotalChannels = m_params.nModules*m_params.nChannels;
	size_t nHits = channelData.size();
	m_params.multiplicity = nHits;
	std::vector<double> cfdTimes(nTotalChannels, std::nan(""));

	// Loop over all hits in event:
	for (size_t i=0; i < nHits; i++){
		// Convenience variable declared to refer to the i^th hit
		const DDASHit& hit 			= channelData[i];
		int slotID				= hit.getSlotID()-2;
		int channelID				= hit.getChannelID();
		int globalChanIdx 			= computeGlobalIndex(hit);
		double cfdTime			= hit.getTime();

		if ((globalChanIdx < 0) || (globalChanIdx >= nTotalChannels)){
			continue;
		}
		
		// ------------------ Saving raw data --------------------
		m_params.chan[globalChanIdx].energy    	= hit.getEnergy(); 	// Output of fast energy filter
		m_params.chan[globalChanIdx].timestamp 	= cfdTime;		// CFD timestamp 
		cfdTimes[globalChanIdx] 		= cfdTime; 
		// ------------------ Saving raw data --------------------

		// ------------- Constructing summary plots --------------
	}
	populateWall(cfdTimes, m_wallA, m_params.nwa);
	populateWall(cfdTimes, m_wallB, m_params.nwb);
}

//________________________________________________________________________
int mapper::computeGlobalIndex(const DDASHit& hit){
	int crateId 		= hit.getCrateID();
	int slotIdx 		= hit.getSlotID() - 2;
	int chanIdx 		= hit.getChannelID();
	const int nChanPerSlot 	= 16;
	return m_chanMap[crateId] + slotIdx*nChanPerSlot + chanIdx;
}
