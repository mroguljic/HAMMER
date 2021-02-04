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
        ~TH2_SF(){};

        RVec<float> eval(float pt, float eta);
    private:
        TFile *file;
        TH2 *h2;
};

TH2_SF::TH2_SF(std::string sf_file,std::string h2Name){
        file = TFile::Open(sf_file.c_str());
        h2 = (TH2*)file->Get(h2Name.c_str());

}


RVec<float> TH2_SF::eval(float pt, float eta) {
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
