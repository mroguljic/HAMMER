#include "TIMBER/Framework/src/JESUncShifter.cc"
#include "ROOT/RVec.hxx"
using namespace ROOT::VecOps;
using LorentzV = ROOT::Math::PtEtaPhiMVector;
using rvec_f = RVec<float> &;


int testJESShifter(std::string jerUncFile){
    JESUncShifter shifter = JESUncShifter(jerUncFile);
    float uncVal = shifter.GetUncVal(0.12, 347.81, 1);//dummy eta, pt values
    //just interested in the uncertainty in this example
    // will apply unc to jets later
    return uncVal;
}