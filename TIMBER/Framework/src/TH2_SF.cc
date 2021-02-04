#include <ROOT/RVec.hxx>
#include "TFile.h"
#include "TH2.h"
#include "TMath.h"
using namespace ROOT::VecOps;

/**
 * @class AK4Btag_SF
 * @brief b tagging scale factor lookup.
 */
class TH2_SF {
    public:

        TH2_SF(std::string sf_file,std::string h2Name);
        TH2_SF(std::string sf_file1,std::string h2Name1,std::string sf_file2,std::string h2Name2,float w1, float w2);
        ~TH2_SF(){};

        RVec<float> eval(float pt, float eta);
        RVec<float> evalComb(float pt, float eta);
    private:
        TFile *file;
        TFile *fileComb;
        TH2 *h2;
        TH2 *h2Comb;
        float relW1;
        float relW2;
};

TH2_SF::TH2_SF(std::string sf_file,std::string h2Name){
    file = TFile::Open(sf_file.c_str());
    h2 = (TH2*)file->Get(h2Name.c_str());

}

TH2_SF::TH2_SF(std::string sf_file1,std::string h2Name1,std::string sf_file2,std::string h2Name2,float w1, float w2){
    file = TFile::Open(sf_file1.c_str());
    h2 = (TH2*)file->Get(h2Name1.c_str());
    fileComb = TFile::Open(sf_file2.c_str());
    h2Comb = (TH2*)file->Get(h2Name2.c_str());
    relW1=w1;
    relW2=w2;
}

RVec<float> TH2_SF::eval(float pt, float eta){
    RVec<float> sf(3);

    int binx = h2->GetXaxis()->FindBin(TMath::Abs(eta));
    int biny = h2->GetYaxis()->FindBin(TMath::Abs(pt));
    if(biny > h2->GetNbinsY()){//avoid overflow in pT
        biny = h2->GetNbinsY();
    }
    float nom       = h2->GetBinContent(binx,biny);
    float errUp     = h2->GetBinErrorUp(binx,biny);
    float errDown   = h2->GetBinErrorLow(binx,biny);

    sf[0] = nom;
    sf[1] = nom-errDown;
    sf[2] = nom+errUp;
    return sf;
};


RVec<float> TH2_SF::evalComb(float pt, float eta){
    //assumes that both th2 have the same binning
    RVec<float> sf(3);

    int binx = h2->GetXaxis()->FindBin(TMath::Abs(eta));
    int biny = h2->GetYaxis()->FindBin(TMath::Abs(pt));
    if(biny > h2->GetNbinsY()){//avoid overflow in pT
        biny = h2->GetNbinsY();
    }
    float nom1       = h2->GetBinContent(binx,biny);
    float errUp1     = h2->GetBinErrorUp(binx,biny);
    float errDown1   = h2->GetBinErrorLow(binx,biny);

    float nom2       = h2Comb->GetBinContent(binx,biny);
    float errUp2     = h2Comb->GetBinErrorUp(binx,biny);
    float errDown2   = h2Comb->GetBinErrorLow(binx,biny);

    sf[0] = nom1*relW1+nom2*relW1;
    sf[1] = (nom1-errDown1)*relW1+(nom2-errDown2)*relW2;
    sf[2] = (nom1+errUp1)*relW1+(nom2+errUp2)*relW2;
    return sf;
};
