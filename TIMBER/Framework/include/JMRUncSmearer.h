// Requires CMSSW
#include <string>
#include <vector>
#include <cstdlib>
#include <TFile.h>
#include <TString.h>
#include "../include/Pythonic.h"
#include <map>
#include <Math/GenVector/LorentzVector.h>
#include <Math/GenVector/PtEtaPhiM4D.h>
#include <TRandom.h>
#include <ROOT/RVec.hxx>
#include <TMath.h>

using namespace ROOT::VecOps;
using rvec_f =  RVec<float>;

using LorentzV = ROOT::Math::PtEtaPhiMVector;

//Class to handle JMS uncertainty shift
class JMRUncSmearer {
    public:

		//TRandom _randGen;

        JMRUncSmearer();
        ~JMRUncSmearer(){};
        float smearMsd(float mSD, float pt,float resSF,rvec_f GenJetAK8_mass, int FatJet_genJetAK8Idx, int shift);//shift 0,1,2: down,nominal,up

};


