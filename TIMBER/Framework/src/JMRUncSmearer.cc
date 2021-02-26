// Requires CMSSW
#include "TIMBER/Framework/include/JMRUncSmearer.h"

JMRUncSmearer::JMRUncSmearer(){
	//this->_randGen = TRandom(1234); //Will be used when stochastic smearing is implemented
}

float JMRUncSmearer::smearMsd(float mSD,float resSF,RVec<float> GenJetAK8_mass, int FatJet_genJetAK8Idx, int shift){
	float scale = 0.;
	if(FatJet_genJetAK8Idx>-1){
		float mass_gen = GenJetAK8_mass[FatJet_genJetAK8Idx];
		scale = 1.0+shift*(resSF-1.0)*(mSD-mass_gen)/mSD;
	}
	else{
		scale = 1.0;
	}

	if(scale<0.){
		return 0.;
	}
	else{
	return mSD*scale;
	}
}