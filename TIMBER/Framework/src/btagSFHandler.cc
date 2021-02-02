#include "TIMBER/Framework/include/PnetSF.h"

btagSFHandler::btagSFHandler(rvec_f wps,rvec_f effs,int year, int var){
	//Only works for 2 wps!
	this->_wps  = wps;
	this->_effs = effs;
	this->_nWps = 2;
	this->_year = year;
	this->_var  = var;
	this->_randGen = TRandom(1234);
}

int btagSFHandler::createTaggingCategories(float taggerVal){
	int cat = 0;
	for(int i=0; i<this->_nWps;i++){
		if(taggerVal>this->_wps[i]){
			cat = i+1;//cat 0 is fail, cat1 is wp1<tag<wp2
		}
	}
	return cat;
}


int btagSFHandler::updateTaggingCategories(int jetCat,float jetPt){
	float sfT   = this->getSF(jetPt,2,this->_year);
	float sfL   = this->getSF(jetPt,1,this->_year);
	int cat;
	if(sfT<1 && sfL<1){
		//If T, check to degrade to L and check if it needs to be further degraded to fail
		if(jetCat==2){
			cat = this->bothLessThanOne(jetCat,sfT,sfL);
			if(cat==1){
				this->bothLessThanOne(cat,sfT,sfL);
			}
			return cat;
		}
		//If L, check if it needs to be degraded
		else if(jetCat==1){
			cat = this->bothLessThanOne(jetCat,sfT,sfL);
			return cat;
		}
		//If fail, return fail
		else{
			return jetCat;
		}
	}
	else if(sfT>1 && sfL>1){
		//If fail, check to upgrade to L and check if it needs to be further upgraded to T
		if(jetCat==0){
			cat = this->bothGreaterThanOne(jetCat,sfT,sfL);
			if(cat==1){
				cat = this->bothGreaterThanOne(cat,sfT,sfL);
			}
			return cat;
		}
		else if(jetCat==1){
			cat = this->bothGreaterThanOne(jetCat,sfT,sfL);
			return cat;			
		}
		else{
			return jetCat;
		}

	}
	else{
		std::cout<<"SF_T>1 and SF_L<1 NOT CODED\n";
		return -1;
	}
}


int btagSFHandler::bothLessThanOne(int jetCat, float sfT, float sfL){
	float freq;
	float rand  = this->_randGen.Rndm();
	if(jetCat==2){
		freq = 1.0-sfT;
		if(rand<freq){
			return 1;//degrade to L
		}
		else{
			return 2;//keep T
		}
	}
	else if(jetCat==1){
		float effT 	= this->_effs[1];
		float effL 	= this->_effs[0];
		float nom = 1-sfL;
		float denom = 1-(effT/effL)*sfT;
		freq = nom/denom;
		if(rand<freq){
			return 0;//degrade to fail
		}
		else{
			return 1;//keep L
		}
	}
	else{
		return 0;
	}
}


int btagSFHandler::bothGreaterThanOne(int jetCat, float sfT, float sfL){
	float freq;
	float rand  = this->_randGen.Rndm();
	float effT 	= this->_effs[1];
	float effL 	= this->_effs[0];
	if(jetCat==1){
		float nom = sfT-1;
		float denom = (effL/effT)*sfT-1;
		freq = nom/denom;
		if(rand<freq){
			return 2;//update to T
		}
		else{
			return 1;//keep L
		}
	}
	else if(jetCat==0){

		float nom = sfL-1.0;
		float denom = (1.0)/effL-1.0;
		freq = nom/denom;
		if(rand<freq){
			return 1;//upgrade to L
		}
		else{
			return 0;//keep fail
		}
	}
	else{
		return 2;
	}
}

float btagSFHandler::getSF(float jetPt,int wpCat, int year){
	//later add option for separate L/T wps
	int ptCat;
	int var = this->_var;
	if(jetPt<600.0){
		ptCat=0;
	}
	else if(jetPt<800.0){
		ptCat=1;
	}
	else{
		ptCat=2;
	}

	if(year==2016){
		return this->SF2016[var][ptCat];
	}
	else if(year==2017){
		return this->SF2017[var][ptCat];
	}
	else{
		return this->SF2018[var][ptCat];
	}

}