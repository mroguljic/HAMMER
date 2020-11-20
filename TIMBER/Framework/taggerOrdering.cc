#include <TRandom.h>
Float_t particleNetHigh(Float_t pnet0,Float_t pnet1);
Float_t particleNetLow(Float_t pnet0,Float_t pnet1);

Float_t particleNetHigh(Float_t pnet0,Float_t pnet1){
    if(pnet0<pnet1){
        return pnet1;
    }
    else{
        return pnet0;
    }
}

Float_t particleNetLow(Float_t pnet0,Float_t pnet1){
    if(pnet0<pnet1){
        return pnet0;
    }
    else{
        return pnet1;
    }
}