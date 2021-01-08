#include "TIMBER/Framework/src/JetUncShifter.cc"

int testJECShifter(std::string jerUncFile){
    JetUncShifter shifter = JetUncShifter(jerUncFile);
    float uncVal = shifter.GetUncVal(0.654, 340.5, 1);
    return uncVal;
}