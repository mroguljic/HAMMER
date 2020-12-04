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

Float_t deltaR(Float_t eta1, Float_t phi1, Float_t eta2, Float_t phi2);
Int_t tightMuonIdx(Int_t nMuon,rvec_b Muon_tightId,rvec_c Muon_pfIsoId);
Int_t tightElectronIdx(Int_t nElectron, rvec_i Electron_cutBased);
Int_t nbAK4(rvec_f btag, rvec_i goodIdxs, Float_t btagCut);
Float_t HTgoodJets(rvec_f Jet_pt, rvec_i goodIdxs);
Int_t leptonGeneration(Int_t skimFlag);
Int_t tightLeptonIdx(Int_t nElectron,rvec_i Electron_cutBased,Int_t nMuon,rvec_b Muon_tightId,rvec_c Muon_pfIsoId,Int_t lGeneration);
Float_t leptonPt(rvec_f Electron_pt,rvec_f Muon_pt,Int_t lIdx,Int_t lGeneration);
Float_t leptonPhi(rvec_f Electron_phi,rvec_f Muon_phi,Int_t lIdx,Int_t lGeneration);
Float_t leptonEta(rvec_f Electron_eta,rvec_f Muon_eta,Int_t lIdx,Int_t lGeneration);
std::vector<int> goodAK4JetIdxs(Int_t nJet,rvec_f Jet_pt, rvec_f Jet_eta, rvec_f Jet_phi, rvec_i Jet_jetId, Float_t phi_l, Float_t eta_l);
Int_t probeAK8JetIdx(Int_t nFatJet,rvec_f FatJet_pt,rvec_f FatJet_msoftdrop,rvec_f FatJet_phi,rvec_f FatJet_eta, rvec_i FatJet_jetId,Float_t lPhi,Float_t lEta);
Int_t classifyProbeJet(Int_t fatJetIdx,rvec_f FatJet_phi,rvec_f FatJet_eta, Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother);
Int_t bFromTopinJet(Float_t FatJet_phi, Float_t FatJet_eta,Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother );
Int_t bFromTopBothinJet(Float_t FatJet_phi, Float_t FatJet_eta,Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother );
Int_t qFromWInJet(Float_t FatJet_phi, Float_t FatJet_eta,Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother );
Int_t qqFromWAllInJet(Float_t FatJet_phi, Float_t FatJet_eta,Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother );
Int_t tagTopPt(Int_t fatJetIdx,rvec_f FatJet_phi,rvec_f FatJet_eta, Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_f GenPart_pt);
Int_t WfromTopPt(Int_t fatJetIdx,rvec_f FatJet_phi,rvec_f FatJet_eta, Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother, rvec_f GenPart_pt);
Float_t deltaRClosestJet(rvec_i goodJetIdxs,rvec_f Jet_eta, rvec_f Jet_phi, Float_t lep_eta, Float_t lep_phi);

Float_t deltaRClosestJet(rvec_i goodJetIdxs,rvec_f Jet_eta, rvec_f Jet_phi, Float_t lep_eta, Float_t lep_phi){
    Float_t minDR = 10.0;
    for(Int_t i=0; i<goodJetIdxs.size();i++){
        Float_t j_eta = Jet_eta[goodJetIdxs[i]];
        Float_t j_phi = Jet_phi[goodJetIdxs[i]];
        Float_t dR = deltaR(j_eta,j_phi,lep_eta,lep_phi);
        if(dR<minDR){
            minDR=dR;
        }
    }
    return minDR;
}


Int_t WfromTopPt(Int_t fatJetIdx,rvec_f FatJet_phi,rvec_f FatJet_eta, Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother, rvec_f GenPart_pt){
    for(Int_t i=0; i<nGenPart;i++){
        Float_t dR = deltaR(GenPart_eta[i],GenPart_phi[i],FatJet_eta[fatJetIdx],FatJet_phi[fatJetIdx]);
        Int_t pid = GenPart_pdgId[i];
        Int_t motherIdx = GenPart_genPartIdxMother[i];
        Int_t motherPid = GenPart_pdgId[motherIdx];
        if(TMath::Abs(pid)==24 && TMath::Abs(motherPid)==6 && dR<0.8){
            return GenPart_pt[i];
        }
    }
    return 0;
}

Int_t tagTopPt(Int_t fatJetIdx,rvec_f FatJet_phi,rvec_f FatJet_eta, Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_f GenPart_pt){
    for(Int_t i=0; i<nGenPart;i++){
        Float_t dR = deltaR(GenPart_eta[i],GenPart_phi[i],FatJet_eta[fatJetIdx],FatJet_phi[fatJetIdx]);
        Int_t pid = GenPart_pdgId[i];
        if(TMath::Abs(pid)==6 && dR<0.8){
            return GenPart_pt[i];
        }
    }
    return 0;
}

Int_t bFromTopinJet(Float_t FatJet_phi, Float_t FatJet_eta,Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother ){
    for(Int_t i=0; i<nGenPart;i++){
        Int_t pid = GenPart_pdgId[i];
        Int_t motherIdx = GenPart_genPartIdxMother[i];
        Int_t motherPid = GenPart_pdgId[motherIdx];

        if(motherPid==-1){
            continue;
        }

        Float_t dR = deltaR(GenPart_eta[i],GenPart_phi[i],FatJet_eta,FatJet_phi);

        if(TMath::Abs(pid)==5 && TMath::Abs(motherPid)==6 && dR<0.8){
            return 1;
        }
    }
    return 0;
}

Int_t bFromTopBothinJet(Float_t FatJet_phi, Float_t FatJet_eta,Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother ){
    for(Int_t i=0; i<nGenPart;i++){
        Int_t pid = GenPart_pdgId[i];
        Int_t motherIdx = GenPart_genPartIdxMother[i];
        Int_t motherPid = GenPart_pdgId[motherIdx];

        if(motherPid==-1){
            continue;
        }

        Float_t dR = deltaR(GenPart_eta[i],GenPart_phi[i],FatJet_eta,FatJet_phi);
        Float_t dRMother = deltaR(GenPart_eta[motherIdx],GenPart_phi[motherIdx],FatJet_eta,FatJet_phi);

        if(TMath::Abs(pid)==5 && TMath::Abs(motherPid)==6 && dR<0.8 && dRMother<0.8){
            return 1;
        }
    }
    return 0;
}

Int_t qFromWInJet(Float_t FatJet_phi, Float_t FatJet_eta,Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother ){
    for(Int_t i=0; i<nGenPart;i++){
        Int_t pid = GenPart_pdgId[i];
        Int_t motherIdx = GenPart_genPartIdxMother[i];
        Int_t motherPid = GenPart_pdgId[motherIdx];

        if(motherPid==-1){
            continue;
        }

        Float_t dR = deltaR(GenPart_eta[i],GenPart_phi[i],FatJet_eta,FatJet_phi);
        if(dR<0.8 && TMath::Abs(pid)<6 && TMath::Abs(pid)>0 && TMath::Abs(motherPid)==24){
            return 1;
        }
    }
    return 0;
}

Int_t qqFromWAllInJet(Float_t FatJet_phi, Float_t FatJet_eta,Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother ){
    Int_t nQ = 0;
    Int_t isWInJet = 0;
    for(Int_t i=0; i<nGenPart;i++){
        Int_t pid = GenPart_pdgId[i];
        Int_t motherIdx = GenPart_genPartIdxMother[i];
        Int_t motherPid = GenPart_pdgId[motherIdx];

        if(motherPid==-1){
            continue;
        }

        Float_t dR = deltaR(GenPart_eta[i],GenPart_phi[i],FatJet_eta,FatJet_phi);
        Float_t dRMother = deltaR(GenPart_eta[motherIdx],GenPart_phi[motherIdx],FatJet_eta,FatJet_phi);
        if(dR<0.8 && TMath::Abs(pid)<6 && TMath::Abs(pid)>0 && TMath::Abs(motherPid)==24 && dRMother<0.8){
            nQ = nQ+1;
            isWInJet = isWInJet+1;
        }
    }
    if(nQ>1 && isWInJet>1){
        return 1;
    }
    else{
        return 0;
    }
}

Int_t classifyProbeJet(Int_t fatJetIdx,rvec_f FatJet_phi,rvec_f FatJet_eta, Int_t nGenPart, rvec_f GenPart_phi,rvec_f GenPart_eta, rvec_i GenPart_pdgId, rvec_i GenPart_genPartIdxMother){
//1: qq, 2: bq, 3:bqq, 0 other
Int_t btInJet = bFromTopinJet(FatJet_phi[fatJetIdx],FatJet_eta[fatJetIdx],nGenPart,GenPart_phi,GenPart_eta,GenPart_pdgId,GenPart_genPartIdxMother);
Int_t bInJet = bFromTopBothinJet(FatJet_phi[fatJetIdx],FatJet_eta[fatJetIdx],nGenPart,GenPart_phi,GenPart_eta,GenPart_pdgId,GenPart_genPartIdxMother);
Int_t qInJet = qFromWInJet(FatJet_phi[fatJetIdx],FatJet_eta[fatJetIdx],nGenPart,GenPart_phi,GenPart_eta,GenPart_pdgId,GenPart_genPartIdxMother);
Int_t qqWInJet = qqFromWAllInJet(FatJet_phi[fatJetIdx],FatJet_eta[fatJetIdx],nGenPart,GenPart_phi,GenPart_eta,GenPart_pdgId,GenPart_genPartIdxMother);

    if(btInJet && qqWInJet){
        return 3;
    }
    else if(bInJet && qInJet){
        return 2;
    }
    else if(qqWInJet){
        return 1;
    }
    else{
        return 0;
    }
}

Int_t probeAK8JetIdx(Int_t nFatJet,rvec_f FatJet_pt,rvec_f FatJet_msoftdrop,rvec_f FatJet_phi,rvec_f FatJet_eta, rvec_i FatJet_jetId,Float_t lPhi,Float_t lEta){
    for(Int_t i=0; i<nFatJet;i++){
        Int_t fatJetReq = FatJet_pt[i]>350 && FatJet_msoftdrop[i]>30 && FatJet_jetId[i]>1;
        Float_t dR = deltaR(FatJet_eta[i],FatJet_phi[i],lEta,lPhi);//opposite hemisphere of the lepton
        if(fatJetReq && dR>2.0){
            return i;
        }
    }
    return -1;
}


Float_t leptonPt(rvec_f Electron_pt,rvec_f Muon_pt,Int_t lIdx,Int_t lGeneration){
    if(lGeneration==1){
        return Electron_pt[lIdx];
    }
    else if(lGeneration==2){
        return Muon_pt[lIdx];
    }
    else{
        return 0;
    }
}

Float_t leptonPhi(rvec_f Electron_phi,rvec_f Muon_phi,Int_t lIdx,Int_t lGeneration){
    if(lGeneration==1){
        return Electron_phi[lIdx];
    }
    else if(lGeneration==2){
        return Muon_phi[lIdx];
    }
    else{
        return 0;
    }
}

Float_t leptonEta(rvec_f Electron_eta,rvec_f Muon_eta,Int_t lIdx,Int_t lGeneration){
    if(lGeneration==1){
        return Electron_eta[lIdx];
    }
    else if(lGeneration==2){
        return Muon_eta[lIdx];
    }
    else{
        return 0;
    }
}

Int_t tightLeptonIdx(Int_t nElectron,rvec_i Electron_cutBased,Int_t nMuon,rvec_b Muon_tightId,rvec_c Muon_pfIsoId,Int_t lGeneration){
    if(lGeneration==1){
        return tightElectronIdx(nElectron,Electron_cutBased);
    }
    else if(lGeneration==2){
        return tightMuonIdx(nMuon,Muon_tightId,Muon_pfIsoId);
    }
    else{
        return -1;
    }
}

Int_t leptonGeneration(Int_t skimFlag){
    if(skimFlag<8 && skimFlag >3){
        return 1;
    }
    else if(skimFlag>7){
        return 2;
    }
    else{
        return 0;
    }
}
Int_t tightMuonIdx(Int_t nMuon,rvec_b Muon_tightId,rvec_c Muon_pfIsoId){
    for(Int_t i=0; i<nMuon;i++){
        if(Muon_tightId[i] && int(Muon_pfIsoId[i])>3){//1=PFIsoVeryLoose, 2=PFIsoLoose, 3=PFIsoMedium, 4=PFIsoTight, 5=PFIsoVeryTight, 6=PFIsoVeryVeryTight
            return i;
        }
    }
    return -1;
}

Int_t tightElectronIdx(Int_t nElectron, rvec_i Electron_cutBased){
    for(Int_t i=0; i<nElectron;i++){
        if(Electron_cutBased[i]>3){//0:fail,1:veto,2:loose,3:medium,4:tight
            return i;
        }
    }
    return -1;
}

std::vector<int> goodAK4JetIdxs(Int_t nJet,rvec_f Jet_pt, rvec_f Jet_eta, rvec_f Jet_phi, rvec_i Jet_jetId, Float_t phi_l, Float_t eta_l){
    std::vector<int> goodIdxs;
    for(Int_t i=0; i<nJet;i++){
        Int_t jetReq = Jet_pt[i]>30 && TMath::Abs(Jet_eta[i])<2.4 && Jet_jetId[i]>1;
        Float_t dR = deltaR(Jet_eta[i],Jet_phi[i],eta_l,phi_l);
        if(jetReq && dR>0.4){
            goodIdxs.push_back(i);
        }
    }
    return goodIdxs;
}

Int_t nbAK4(rvec_f btag, rvec_i goodIdxs, Float_t btagCut){
    Int_t n=0;
    for(int i = 0; i < goodIdxs.size(); i++){
        Int_t idx = goodIdxs[i];
        if(btag[idx]>btagCut){
            n = n+1;
        }
    }
    return n;
}

Float_t HTgoodJets(rvec_f Jet_pt, rvec_i goodIdxs){
    Float_t HT = 0;
    for(int i = 0; i < goodIdxs.size(); i++){
        Int_t idx = goodIdxs[i];
        HT = HT+Jet_pt[idx];
    }
    return HT;
}