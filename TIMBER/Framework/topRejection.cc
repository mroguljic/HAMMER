#define _USE_MATH_DEFINES

#include <cmath>
#include "ROOT/RVec.hxx"
using namespace ROOT::VecOps;
using rvec_f = const RVec<float> &;
//Tries to find ak4 jet in the opposite hemisphere of the ak8 jet which passes
//the btag cut. Returns 0 if it doesn't find it, 1 if it does
Int_t topRejection(rvec_f FJphi, rvec_f FJmass, unsigned int FJnjets, rvec_f Jphi, unsigned int Jnjets, rvec_f btagDeepB, Float_t btagCut){
    //First find the highest pt ak8 jet with mass > 40 geV
    auto candidateFatJetIndex = -1;
    for (unsigned int i =0; i<FJnjets; i++){
        if (FJmass[i] > 40) {
            candidateFatJetIndex = i;
            break;
        }
    }
    if (candidateFatJetIndex == -1){
        return 0;
    }

    RVec<int> candidateJetIndices;

    if (Jnjets < 1){
        return 0;
    }

    if(Jnjets>4){
        Jnjets=4;
    }

    for (unsigned int ijet = 0; ijet<Jnjets; ijet++){
        if (abs(FJphi[candidateFatJetIndex]-Jphi[ijet]) > M_PI ){
            if(btagDeepB[ijet]>btagCut && btagDeepB[ijet] <= 1.0){
                return 1;
            }
        }
    }
    return 0;

}
