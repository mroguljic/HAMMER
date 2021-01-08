// Requires CMSSW
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <math.h>
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include <TRandom3.h>
#include <TFile.h>
#include <TF1.h>
#include <TString.h>
#include <TSystem.h>
#include <ROOT/RVec.hxx>
#include <Math/GenVector/LorentzVector.h>
#include <Math/GenVector/PtEtaPhiM4D.h>
#include <Math/Vector4Dfwd.h>
#include "../include/Pythonic.h"

using namespace ROOT::VecOps;
using LorentzV = ROOT::Math::PtEtaPhiMVector;

//Class to handle JES uncertainty shift
//Constructor initializes the reader
//GetUncVal calculates the uncertainty depending on the eta and pt
class JESUncShifter {
    public:

        JESUncShifter(std::string jesUncFile_);
        ~JESUncShifter(){};
        float GetUncVal(float eta, float pt, int unc_shift);

        boost::shared_ptr<JetCorrectionUncertainty> ptr_jesUnc; 
};


