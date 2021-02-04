// Requires CMSSW
#include "TIMBER/Framework/include/JMSUncShifter.h"

JMSUncShifter::JMSUncShifter(){
	this->jmsTable["2016"]={1.00, 0.95, 1.05};//nominal, down, up
	this->jmsTable["2017"]={1.00, 0.95, 1.05};
	this->jmsTable["2018"]={1.00, 0.95, 1.05};
}

float JMSUncShifter::shiftMsd(float mSD,std::string year,int shift){
	float jmsVal = jmsTable[year][shift];
	return mSD*jmsVal;
}