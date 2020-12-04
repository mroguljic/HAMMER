#include <TFile.h>
#include <TMath.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "ROOT/RVec.hxx"

using namespace ROOT::VecOps;
using rvec_i = const RVec<int> &;
using rvec_f = const RVec<float> &;
using rvec_c = const RVec<char> &;
using rvec_b = const RVec<bool> &;
using LVector = ROOT::Math::PtEtaPhiMVector;

Int_t highHTFlag(Int_t nGenPart, rvec_i GenPart_pdgId, rvec_f GenPart_pt, rvec_f GenPart_phi, rvec_f GenPart_eta, rvec_f GenPart_mass, Int_t nGenJetAK8,rvec_f GenJetAK8_pt, rvec_f GenJetAK8_phi, rvec_f GenJetAK8_eta, rvec_f GenJetAK8_mass);
Float_t deltaR(Float_t eta1, Float_t phi1, Float_t eta2, Float_t phi2);

Int_t highHTFlag(Int_t nGenPart, rvec_i GenPart_pdgId, rvec_f GenPart_pt, rvec_f GenPart_phi, rvec_f GenPart_eta, rvec_f GenPart_mass, Int_t nGenJetAK8,rvec_f GenJetAK8_pt, rvec_f GenJetAK8_phi, rvec_f GenJetAK8_eta, rvec_f GenJetAK8_mass){
    if(nGenJetAK8<1){
        return 0;
    }
    std::vector<LVector> jetVectors;
    std::vector<LVector> nuVectors;

    for(Int_t i=0;i<nGenPart;i++){
        if(GenPart_pdgId[i]==12 || GenPart_pdgId[i]==14 || GenPart_pdgId[i]==16){
            nuVectors.push_back(LVector(GenPart_pt[i],GenPart_eta[i],GenPart_phi[i],GenPart_mass[i]));
        }
    }   

    for(Int_t i=0;i<nGenJetAK8;i++){
    //pT ordering of jets can change with adding neutrinos
    //still only considering first two leading jets
        if(i>1){
            break;
        }
        jetVectors.push_back(LVector(GenJetAK8_pt[i],GenJetAK8_eta[i],GenJetAK8_phi[i],GenJetAK8_mass[i]));
    }

    for(auto& nuVector: nuVectors){
        for(auto& jetVector: jetVectors){
            Float_t dR = deltaR(nuVector.Eta(),nuVector.Phi(),jetVector.Eta(),jetVector.Phi());
            if(dR<0.8){
                jetVector = jetVector+nuVector;
            }
        }
    }
    Float_t highestpT = 0;
    for(const auto& jetVector: jetVectors){
        Float_t pT = jetVector.Pt();
        if(pT>highestpT){
            highestpT=pT;
        }
    }

    if(highestpT>649.0){
        return 1;
    }
    else{
        return 0;
    }

}