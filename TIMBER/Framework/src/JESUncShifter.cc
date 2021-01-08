// Requires CMSSW
#include "TIMBER/Framework/include/JESUncShifter.h"


JESUncShifter::JESUncShifter(std::string jesUncFile_){
	ptr_jesUnc = boost::shared_ptr<JetCorrectionUncertainty>(new JetCorrectionUncertainty(jesUncFile_) );
}

float JESUncShifter::GetUncVal(float eta, float pt, int unc_shift){
	float unc = 0.;
	if(unc_shift!=0){
		std::cout<<"Eta: "<<eta<<" | pT: "<<pt<<"\n";
		ptr_jesUnc->setJetEta(eta);
		ptr_jesUnc->setJetPt(pt);
		unc = ptr_jesUnc->getUncertainty(true);
	}
	return unc;
}