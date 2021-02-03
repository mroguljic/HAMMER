// without CMSSW / standalone:
#include "TIMBER/Framework/ExternalTools/BTagCalibrationStandalone.h"
#include "TIMBER/Framework/ExternalTools/BTagCalibrationStandalone.cpp"
#include <ROOT/RVec.hxx>

using namespace ROOT::VecOps;

/**
 * @class AK4Btag_SF
 * @brief Subjet b tagging scale factor lookup.
 */
class AK4Btag_SF {
    public:
        /**
         * @brief Construct a new subjet b tag scale factor lookup object
         * 
         * @param year 16, 17, or 18.
         * @param tagger Ex. DeepCSV. See TIMBER/data/OfficialSFs/ for others.
         * @param op_string "loose", "medium", "tight"
         */
        AK4Btag_SF(int year, std::string tagger, std::string op_string);//loose, medium, tight, reshaping
        ~AK4Btag_SF(){};
        /**
         * @brief Per-event evaluation function
         * 
         * @param pt \f$p_{T}\f$ of subjet
         * @param eta \f$\eta\f$ of subjet
         * @return RVec<float> Nominal, up, down scale factor values.
         */
/*        RVec<float> eval(float pt, float eta);*/
    
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
            csv_file = std::string(std::getenv("TIMBERPATH"))+"TIMBER/data/OfficialSFs/DeepJet_102XSF_V2";
        }

        std::cout<<csv_file<<"\n";
        // setup calibration + reader
        calib = BTagCalibration(tagger, csv_file);
        std::cout<<"Created calib\n";
        reader = BTagCalibrationReader(operating_point,  // operating point
                                     "central",             // central sys type
                                     {"up", "down"});      // other sys types
        std::cout<<"Created reader\n";
        reader.load(calib,                // calibration instance
                    BTagEntry::FLAV_B,    // btag flavour
                    "comb");               // measurement type
        std::cout<<"Loaded reader\n";

}


/*RVec<float> AK4Btag_SF::eval(float pt, float eta) {
    // Note: this is for b jets, for c jets (light jets) use FLAV_C (FLAV_UDSG)
    // auto sf_lookup = [this](float eta, float pt){
    //     std::vector<float> v;

    //     v.push_back(reader.eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt));
    //     v.push_back(reader.eval_auto_bounds("up", BTagEntry::FLAV_B, eta, pt));
    //     v.push_back(reader.eval_auto_bounds("down", BTagEntry::FLAV_B, eta, pt));

    //     return v;
    // };

    // auto jet_scalefactor = Map(pt_vec, eta_vec, sf_lookup);

    RVec<float> jet_scalefactor(3);

    float nom = reader.eval_auto_bounds("central", BTagEntry::FLAV_B, eta, pt);
    float up = reader.eval_auto_bounds("up", BTagEntry::FLAV_B, eta, pt);
    float down = reader.eval_auto_bounds("down", BTagEntry::FLAV_B, eta, pt);

    jet_scalefactor[0] = nom;
    jet_scalefactor[1] = up;
    jet_scalefactor[2] = down;

    return jet_scalefactor;
};*/
