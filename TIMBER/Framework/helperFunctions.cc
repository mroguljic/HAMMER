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

Float_t calculateHT(Int_t nJet, rvec_f Jet_eta, rvec_f Jet_pt, Float_t pt_cut, Float_t eta_cut);
//pT sum of AK4 jets with pT > 30 GeV and |eta| < 2.4
Float_t nAK4(Int_t nJet, rvec_f Jet_eta, rvec_f Jet_pt, Float_t pt_cut, Float_t eta_cut);
//Number of AK4 jets with pT > pt_cut GeV and |eta| < eta_cut
Float_t n_bAK4(Int_t nJet, rvec_f Jet_eta, rvec_f Jet_phi, rvec_f Jet_pt, rvec_f Jet_btag, Float_t btag_cut, Int_t nFatJet, rvec_f FatJet_eta, rvec_f FatJet_phi);
//Number of b-tagged AK4 jets (pT > 30 GeV |eta| < 2.4) separated from the 2 leading AK8 jets
Float_t n_nonbAK4(Int_t nJet, rvec_f Jet_eta, rvec_f Jet_phi, rvec_f Jet_pt, rvec_f Jet_btag, Float_t btag_cut, Int_t nFatJet, rvec_f FatJet_eta, rvec_f FatJet_phi);
//Number of non b-tagged AK4 jets (pT > 30 GeV |eta| < 2.4) separated from the 2 leading AK8 jets
Float_t deltaR(Float_t eta1, Float_t phi1, Float_t eta2, Float_t phi2);
//deltaR between two vectors
Int_t skimmingLeadingAK8Jet(Int_t nFatJet, rvec_f FatJet_eta, rvec_f FatJet_pt, rvec_f FatJet_msoftdrop);
//Returns 1 if Leading AK8 jet(pT> 300 GeV) + eta(< 2.5) + softdrop_mass(> 30 GeV)
Int_t skimmingTwoAK8Jets(Int_t nFatJet, rvec_f FatJet_eta, rvec_f FatJet_pt, rvec_f FatJet_msoftdrop);
//Returns 1 if Two leading AK8 jets pt(> 300 GeV) + eta(< 2.5) + softdrop_mass(> 30 GeV)
Int_t skimmingIsoLepton(Int_t nJet, rvec_f Jet_eta, rvec_f Jet_pt, Int_t nElectron, rvec_i Electron_cutBased,Int_t nMuon, rvec_b Muon_looseId,rvec_c Muon_pfIsoId);
//Returns 1(2) if Single isolated electron(muon)(pT > 30 GeV, eta < 2.5, loose iso) + AK4 jet (pt > 30 GeV, eta < 2.5) exist
Int_t goodElectron(Int_t nElectron, rvec_i Electron_cutBased);
//Returns 1 if isolated loose electron exists
Int_t goodMuon(Int_t nMuon, rvec_b Muon_looseId,rvec_c Muon_pfIsoId);
//Returns 1 if isolated loose muon exists
Int_t skimFlag(Int_t nFatJet, rvec_f FatJet_eta, rvec_f FatJet_pt, rvec_f FatJet_msoftdrop,Int_t nJet, rvec_f Jet_eta, rvec_f Jet_pt, Int_t nElectron, rvec_i Electron_cutBased,Int_t nMuon, rvec_b Muon_looseId,rvec_c Muon_pfIsoId);

Int_t skimFlag(Int_t nFatJet, rvec_f FatJet_eta, rvec_f FatJet_pt, rvec_f FatJet_msoftdrop,Int_t nJet, rvec_f Jet_eta, rvec_f Jet_pt, Int_t nElectron, rvec_i Electron_cutBased,Int_t nMuon, rvec_b Muon_looseId,rvec_c Muon_pfIsoId){
    Int_t jetSkim    = skimmingLeadingAK8Jet(nFatJet,FatJet_eta,FatJet_pt,FatJet_msoftdrop);
    Int_t dijetSkim  = skimmingTwoAK8Jets(nFatJet,FatJet_eta,FatJet_pt,FatJet_msoftdrop);
    Int_t leptonSkim = skimmingIsoLepton(nJet,Jet_eta,Jet_pt,nElectron,Electron_cutBased,nMuon,Muon_looseId,Muon_pfIsoId);//1 if ele, 2 if muon
    //Bits: 1:jetSkim, 2:dijetSkim, 4:electronSkim,8:muonSkim
    Int_t skimScore  = jetSkim+2*dijetSkim+4*leptonSkim;
    return skimScore;
}


Int_t goodMuon(Int_t nMuon, rvec_b Muon_looseId,rvec_c Muon_pfIsoId){
    for(Int_t i=0; i<nMuon;i++){
        if(Muon_looseId[i] && int(Muon_pfIsoId[i])>1){//1=PFIsoVeryLoose, 2=PFIsoLoose, 3=PFIsoMedium, 4=PFIsoTight, 5=PFIsoVeryTight, 6=PFIsoVeryVeryTight
            return 1;
        }
    }
    return 0;
}
Int_t goodElectron(Int_t nElectron, rvec_i Electron_cutBased){
    for(Int_t i=0; i<nElectron;i++){
        if(Electron_cutBased[i]>1){//0:fail,1:veto,2:loose,3:medium,4:tight
            return 1;
        }
    }
    return 0;
}


Int_t skimmingIsoLepton(Int_t nJet, rvec_f Jet_eta, rvec_f Jet_pt, Int_t nElectron, rvec_i Electron_cutBased,Int_t nMuon, rvec_b Muon_looseId,rvec_c Muon_pfIsoId){
    if(nJet<1){
        return 0;
    }
    Int_t isGoodJet = TMath::Abs(Jet_eta[0])>2.5 && Jet_pt[0]>30;
    Int_t isGoodMuon = goodMuon(nMuon,Muon_looseId,Muon_pfIsoId);
    Int_t isGoodElectron = goodElectron(nElectron,Electron_cutBased);

    if(isGoodElectron && isGoodMuon){
        return 0;
    }
    else if (isGoodJet && isGoodElectron){
        return 1;
    }
    else if (isGoodJet && isGoodMuon){
        return 2;
    }
    else{
        return 0;
    }
}


Int_t skimmingLeadingAK8Jet(Int_t nFatJet, rvec_f FatJet_eta, rvec_f FatJet_pt, rvec_f FatJet_msoftdrop){
    if(nFatJet<1){
        return 0;
    }
    else if(TMath::Abs(FatJet_eta[0])<2.5 && FatJet_pt[0]>400 && FatJet_msoftdrop[0]>30){
        return 1;
    }
    else{
    return 0;
    }
}

Int_t skimmingTwoAK8Jets(Int_t nFatJet, rvec_f FatJet_eta, rvec_f FatJet_pt, rvec_f FatJet_msoftdrop){
    if(nFatJet<2){
        return 0;
    }
    Int_t pT = FatJet_pt[0]>300 && FatJet_pt[1]>300;
    Int_t eta = TMath::Abs(FatJet_eta[0]<2.5) && TMath::Abs(FatJet_eta[1])<2.5;
    Int_t mSD = FatJet_msoftdrop[0]>30 && FatJet_msoftdrop[1]>30;
    
    if(pT && eta && mSD){
        return 1;
    }
    else{
        return 0;
    }
}


Float_t calculateHT(Int_t nJet, rvec_f Jet_eta, rvec_f Jet_pt,Float_t pt_cut, Float_t eta_cut){
    Float_t HT = 0.;
    for(Int_t i=0; i<nJet;i++){
        if(Jet_pt[i]<pt_cut){
            return HT;
        }
        else if(TMath::Abs(Jet_eta[i])>eta_cut){
            continue;
        }
        else{
            HT = HT + Jet_pt[i];
        }
    }
    return HT;
}

Float_t nAK4(Int_t nJet, rvec_f Jet_eta, rvec_f Jet_pt, Float_t pt_cut, Float_t eta_cut){
    Int_t n = 0;
    for(Int_t i=0; i<nJet;i++){
        if(Jet_pt[i]<pt_cut){
            return n;
        }
        else if(TMath::Abs(Jet_eta[i])>eta_cut){
            continue;
        }
        else{
            n = n +1;
        }
    }
    return n;
}

Float_t n_bAK4(Int_t nJet, rvec_f Jet_eta, rvec_f Jet_phi, rvec_f Jet_pt, rvec_f Jet_btag, Float_t btag_cut, Int_t nFatJet, rvec_f FatJet_eta, rvec_f FatJet_phi){
    Int_t n = 0;
    for(Int_t i=0; i<nJet;i++){
        if(Jet_pt[i]<30.0){
            return n;
        }
        else if(TMath::Abs(Jet_eta[i])>2.4){
            continue;
        }
        else if(Jet_btag[i]<btag_cut){
            continue;
        }
        else if(deltaR(Jet_eta[i],Jet_phi[i],FatJet_eta[0],FatJet_phi[0])<0.8){
            continue;
        }
        else if(deltaR(Jet_eta[i],Jet_phi[i],FatJet_eta[1],FatJet_phi[1])<0.8){
            continue;
        }
        else{
            n = n +1;
        }
    }
    return n;
}

Float_t n_nonbAK4(Int_t nJet, rvec_f Jet_eta, rvec_f Jet_phi, rvec_f Jet_pt, rvec_f Jet_btag, Float_t btag_cut, Int_t nFatJet, rvec_f FatJet_eta, rvec_f FatJet_phi){
    Int_t n = 0;
    for(Int_t i=0; i<nJet;i++){
        if(Jet_pt[i]<30.0){
            return n;
        }
        else if(TMath::Abs(Jet_eta[i])>2.4){
            continue;
        }
        else if(Jet_btag[i]>btag_cut){
            continue;
        }
        else if(deltaR(Jet_eta[i],Jet_phi[i],FatJet_eta[0],FatJet_phi[0])<0.8){
            continue;
        }
        else if(deltaR(Jet_eta[i],Jet_phi[i],FatJet_eta[1],FatJet_phi[1])<0.8){
            continue;
        }
        else{
            n = n +1;
        }
    }
    return n;
}
