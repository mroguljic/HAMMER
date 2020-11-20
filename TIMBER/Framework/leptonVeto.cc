#include <TFile.h>
#include <TMath.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "ROOT/RVec.hxx"
using namespace ROOT::VecOps;
using rvec_i = const RVec<int> &;
using rvec_b = const RVec<bool> &;
using rvec_c = const RVec<char> &;

Int_t leptonVeto(Int_t nElectron, rvec_i Electron_cutBased, Int_t nMuon, rvec_b Muon_looseId,Int_t nTau,rvec_c Tau_idMVAnewDM2017v2){
//Returns 0 if no loose leptons found in event, 1 otherwise
//Tau_idMVAnewDM2017v2: bitmask 1 = VVLoose, 2 = VLoose, 4 = Loose, 8 = Medium, 16 = Tight, 32 = VTight, 64 = VVTight*
//Electron_cutBased : Int_t cut-based ID Fall17 V2 (0:fail, 1:veto, 2:loose, 3:medium, 4:tight) 
	if(nElectron >= 1){
		for(int i=0; i<nElectron; i++){
			if(Electron_cutBased[i] >= 2){
				return 1;
			}
		}
	}
	if(nMuon >= 1){
		for(int i=0; i<nMuon; i++){
			if(Muon_looseId[i]){
				return 1;
			}
		}
	}

	if(nTau>=1){
		for(int i=0; i<nTau; i++){
			if(Tau_idMVAnewDM2017v2[i]>=4){
				return 1;
			}
		}
	}
	return 0;
}