// without CMSSW / standalone:
#include "TIMBER/Framework/ExternalTools/BTagCalibrationStandalone.h"
#include "TIMBER/Framework/ExternalTools/BTagCalibrationStandalone.cpp"
#include <ROOT/RVec.hxx>

using namespace ROOT::VecOps;

/**
 * @class AK4Btag_SF
 * @brief b tagging scale factor lookup.
 */
class AK4Btag_SF {
    public:
        /**
         * @brief Construct a new b tag scale factor lookup object
         * 
         * @param year 16, 17, or 18.
         * @param tagger Ex. DeepCSV, DeepJet
         * @param op_string "loose", "medium", "tight", "reshaping"
         */
        AK4Btag_SF(int year, std::string tagger, std::string op_string);
        ~AK4Btag_SF(){};
        /**
         * @brief Per-event evaluation function
         * 
         * @param pt \f$p_{T}\f$ of subjet
         * @param eta \f$\eta\f$ of subjet
         * @return RVec<float> Nominal, up, down scale factor values.
         */
        RVec<float> eval(float pt, float eta,int flav, float disc);
        RVec<float> evalCollection(int nJet, RVec<float> pt, RVec<float> eta, RVec<float> flav, RVec<float> disc,std::string var);
    private:
        std::string csv_file;
        BTagEntry::OperatingPoint operating_point;
        BTagCalibration calib;
        BTagCalibrationReader reader;
};

AK4Btag_SF::AK4Btag_SF(int year, std::string tagger, std::string op_string){
		std::cout<<tagger<<" "<<year<<" "<<op_string<<"\n";
        if (op_string == "loose") {
            operating_point = BTagEntry::OP_LOOSE;
        } else if (op_string == "medium") {
            operating_point = BTagEntry::OP_MEDIUM;
        } else if (op_string == "tight") {
            operating_point = BTagEntry::OP_TIGHT;
        } else if (op_string == "reshaping") {
            operating_point = BTagEntry::OP_RESHAPING;
        }
        else{
            throw "Operating point type not supported!";
        }

        if (year == 16) {
            csv_file = std::string(std::getenv("TIMBERPATH"))+"TIMBER/data/OfficialSFs/DeepJet_2016LegacySF_V1.csv";
        } else if (year == 17) {
            csv_file = std::string(std::getenv("TIMBERPATH"))+"TIMBER/data/OfficialSFs/DeepFlavour_94XSF_V4_B_F.csv";
        } else if (year == 18) {
            csv_file = std::string(std::getenv("TIMBERPATH"))+"TIMBER/data/OfficialSFs/DeepJet_102XSF_V2.csv";
        }

        std::cout<<csv_file<<"\n";
        // setup calibration + reader
        calib = BTagCalibration(tagger, csv_file);
        std::cout<<"Created calib\n";
        reader = BTagCalibrationReader(operating_point,  // operating point
                                     "central",             // central sys type
                                     {"up_hf", "down_hf"});      // other sys types
        std::cout<<"Created reader\n";
        reader.load(calib,                // calibration instance
                    BTagEntry::FLAV_B,    // btag flavour
                    "iterativefit");               // measurement type
        std::cout<<"Loaded reader\n";

}


RVec<float> AK4Btag_SF::eval(float pt, float eta, int flav, float disc) {
    RVec<float> jet_scalefactor(3);

    BTagEntry::JetFlavor fl = static_cast<BTagEntry::JetFlavor>(flav);

    float nom = reader.eval_auto_bounds("central", fl, eta, pt, disc);//eta, pt, discr
    float up = reader.eval_auto_bounds("up_hf", fl, eta, pt, disc);
    float down = reader.eval_auto_bounds("down_hf", fl, eta, pt, disc);

    jet_scalefactor[0] = nom;
    jet_scalefactor[1] = up;
    jet_scalefactor[2] = down;

    return jet_scalefactor;
};

RVec<float> AK4Btag_SF::evalCollection(int nJet, RVec<float> pt, RVec<float> eta, RVec<float> flav, RVec<float> disc,std::string var) {
    RVec<float> recalib_disc(nJet);
     BTagEntry::JetFlavor fl;
    for(int i=0; i<nJet;i++){
        if(flav[i]==5){
            fl = static_cast<BTagEntry::JetFlavor>(2);//b
        }
        else if(flav[i]==3){
            fl = static_cast<BTagEntry::JetFlavor>(1);//c
        }
        else{
            fl = static_cast<BTagEntry::JetFlavor>(0);//udsg
        }
        float sf = reader.eval_auto_bounds(var, fl, eta[i], pt[i], disc[i]);//var= central, up_hf, down_hf
        recalib_disc[i] = sf*disc[i];
    }

    return recalib_disc;
};