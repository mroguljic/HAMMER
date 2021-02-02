// Requires CMSSW
#include "TIMBER/Framework/include/JMRUncSmearer.h"

JMRUncSmearer::JMRUncSmearer(){
	this->_randGen = TRandom(1234);
}

float JMRUncSmearer::smearMsd(float mass, float pt,float sigma,rvec_f GenJetAK8_pt, int FatJet_genJetAK8Idx){
	float scale;
	if(FatJet_genJetAK8Idx>-1){
		float pt_gen = GenJetAK8_pt[FatJet_genJetAK8Idx];
		scale = 1.0+(sigma-1.0)*(pt-pt_gen)/pt;
	}
	else{
		float rand = this->_randGen.Gaus(0.,0.1);//0.1 should be replaced with real pt resolution
		if(sigma*sigma-1<0){
			scale = 1.0;
		}
		else{
			scale = 1.0+_randGen.Gaus(0,0.1)*TMath::Sqrt(sigma*sigma-1);
		}
	}

	if(scale<0.){
		return 0.0;
	}
	else{
	return mass*scale;
	}
}