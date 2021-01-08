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


class JetUncShifter {
    public:

        JetUncShifter(std::string jerUncFile_);
        ~JetUncShifter(){};
        float GetUncVal(float eta, float pt, int unc_shift);

        std::string jerUncFile_;
        boost::shared_ptr<JetCorrectionUncertainty> ptr_jecUnc; 
};


