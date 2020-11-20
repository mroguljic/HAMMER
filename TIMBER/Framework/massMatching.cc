#include <TMath.h>
#include <stdio.h>
#include <iostream>
#include <TRandom.h>

int getRand01(){
    //randomly returns 0 or 1
    static long seed = 1;
    seed++;
    TRandom randGen = TRandom(seed);
    int rand = randGen.Binomial(1,0.5);
    return rand;   
}


Int_t higgsMassMatching(Float_t m0, Float_t m1);

Int_t higgsMassMatching(Float_t m0, Float_t m1){
//returns index of the jet closer in mass to Higgs jet SD mass (~115)
	Float_t diff0 = TMath::Abs(m0-115.0);
	Float_t diff1 = TMath::Abs(m1-115.0);
	if(diff0<=diff1){
		return 0;
	}
	else{
		return 1;
	}
}

Int_t higgsMassMatchingAlt(Float_t m0, Float_t m1){

	int HcandidateFlag0 = 0;
	int HcandidateFlag1 = 0;

	if(m0 > 110 && m0 < 140 ){
		HcandidateFlag0 = 1;
	}
	if(m1 > 110 && m1 < 140 ){
		HcandidateFlag1 = 1;
	}
	if(HcandidateFlag0==1 && HcandidateFlag1!=1){
		return 0;
	}
	if(HcandidateFlag1==1 && HcandidateFlag0!=1){
		return 1;
	}
	if(HcandidateFlag0==1 && HcandidateFlag1==1){
		int res = getRand01();
		return res;
	}
	return -1;
}
