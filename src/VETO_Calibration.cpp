// C/C++ includes
#include <random>

// Project includes
#include <config.h>
#include "VETO_Calibration.h"
#include "parameters.h"

//________________________________________________________________________
// Initialize our parameters and variables. 
//
VETO_Calibration::VETO_Calibration(parameters& rParams) :
	m_params(rParams),
	m_ecal("cal", 32768, 0, 32767, "a.u", 16, 0),
	m_slope("slope", 0.5, "", 16, 0),
	m_offset("offset", 1000., "", 16, 0)
{}

//________________________________________________________________________
// Each physics event is processed here.
//
Bool_t VETO_Calibration::operator()(
		Address_t pEvent, CEvent& rEvent,
		CAnalyzer& rAnalyzer, CBufferDecoder& rDecoder){
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> dist(0., 1.);
	
	for (int i = 0; i < 16; i++){
		if (m_params.chan[i].energy.isValid()){
			m_ecal[i] = m_slope[i]*m_params.chan[i].energy
				+ m_offset[i] + dist(gen);
		}
	}
	
	return kfTRUE;
}
