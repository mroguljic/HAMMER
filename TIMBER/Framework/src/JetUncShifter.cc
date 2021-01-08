// Requires CMSSW
#include "TIMBER/Framework/include/JetUncShifter.h"
using namespace ROOT::VecOps;
using LorentzV = ROOT::Math::PtEtaPhiMVector;


JetUncShifter::JetUncShifter(std::string jerUncFile_){
	ptr_jecUnc = boost::shared_ptr<JetCorrectionUncertainty>(new JetCorrectionUncertainty(jerUncFile_) ) ;
}

float JetUncShifter::GetUncVal(float eta, float pt, int unc_shift){
	float unc = 0.;
	if(unc_shift!=0){
		std::cout<<"Eta: "<<eta<<" | pT: "<<pt<<"\n";
		ptr_jecUnc->setJetEta(eta);
		ptr_jecUnc->setJetPt(pt);
		unc = ptr_jecUnc->getUncertainty(true);
	}
	return unc;
}