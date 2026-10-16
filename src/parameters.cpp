// Project includes
#include "parameters.h"

//________________________________________________________________________
// Initialize channel data for Pixie 16 (500 MSPS 14-bit ADC)
//
void ChannelData::Initialize(std::string name) {
	// Energy: 32768 channels between 0 and 32767
	energy.Initialize(
			name + ".energy", 16384, 0, 16384, "a.u."
	);
	
	// Timestamp: 64 bits with 2^64-1 ns per bin starting at 0
	timestamp.Initialize(
			name + ".cfd", 64, 0, std::pow(2,64)-1, "ns", true
	);
}

void BarData::Initialize(std::string name){
	timeDiff.Initialize(name + ".timeDiff", 2048, -20, 20, "ns", true);
}

//________________________________________________________________________
// Creates parameters! 
// If you want a different parameter to be viewed in Spectcl, you have to define it here
// The point of the tcl script is to create spectra that REFERENCE these parameters
parameters::parameters(std::string name){
	// Create the 16 channels 
	for (size_t mod=0; mod < nModules; mod++){
		for (size_t i=0; i < nChannels; i++) {
			std::ostringstream ss;
			ss << name << "."
				<< std::setw(2) << std::setfill('0') << mod << "."
				<< std::setw(2) << std::setfill('0') << i;

			chan[mod*nChannels + i].Initialize(ss.str());
		}
	}
	
	// Initialize the multiplicity: 32 bins between 0 and 31
	int maxMult = nModules*(nChannels);
	multiplicity.Initialize(name + ".mult", maxMult, 0, maxMult-1, "a.u.");
	for (int i=0; i < nBars; i++){
		std::ostringstream a;
		a << "NWA." << std::setw(2) << std::setfill('0') << i;
		nwa[i].Initialize(a.str());

		std::ostringstream b;
		b << "NWB." << std::setw(2) << std::setfill('0') << i;
		nwb[i].Initialize(b.str());
	}
}
