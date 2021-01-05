#include <map>
#include <string>
#include <vector>
using namespace std;

namespace JME {

    // JEC dict
    map<string,string> jecTagsMC = {
        {"2016", "Summer16_07Aug2017_V11_MC"},
        {"2017", "Fall17_17Nov2017_V32_MC"},
        {"2018", "Autumn18_V19_MC"},
        {"UL2017", "Summer19UL17_V5_MC"},
    };

    map<string,string> jecTagsFastSim = {
        {"2016", "Spring16_25nsFastSimV1_MC"},
        {"2017", "Fall17_FastSimV1_MC"},
        {"2018", "Autumn18_FastSimV1_MC"}
    };

    map<string,string> archiveTagsDATA = {
        {"2016", "Summer16_07Aug2017_V11_DATA"},
        {"2017", "Fall17_17Nov2017_V32_DATA"},
        {"2018", "Autumn18_V19_DATA"},
        {"UL2017", "Summer19UL17_V5_DATA"}
    };

    map<string,string> jecTagsDATA = {
        {"2016B", "Summer16_07Aug2017BCD_V11_DATA"},
        {"2016C", "Summer16_07Aug2017BCD_V11_DATA"},
        {"2016D", "Summer16_07Aug2017BCD_V11_DATA"},
        {"2016E", "Summer16_07Aug2017EF_V11_DATA"},
        {"2016F", "Summer16_07Aug2017EF_V11_DATA"},
        {"2016G", "Summer16_07Aug2017GH_V11_DATA"},
        {"2016H", "Summer16_07Aug2017GH_V11_DATA"},
        {"2017B", "Fall17_17Nov2017B_V32_DATA"},
        {"2017C", "Fall17_17Nov2017C_V32_DATA"},
        {"2017D", "Fall17_17Nov2017DE_V32_DATA"},
        {"2017E", "Fall17_17Nov2017DE_V32_DATA"},
        {"2017F", "Fall17_17Nov2017F_V32_DATA"},
        {"2018A", "Autumn18_RunA_V19_DATA"},
        {"2018B", "Autumn18_RunB_V19_DATA"},
        {"2018C", "Autumn18_RunC_V19_DATA"},
        {"2018D", "Autumn18_RunD_V19_DATA"},
        {"UL2017B", "Summer19UL17_RunB_V5_DATA"},
        {"UL2017C", "Summer19UL17_RunC_V5_DATA"},
        {"UL2017D", "Summer19UL17_RunD_V5_DATA"},
        {"UL2017E", "Summer19UL17_RunE_V5_DATA"},
        {"UL2017F", "Summer19UL17_RunF_V5_DATA"}
    };

    map<string,string> jerTagsMC = {
        {"2016", "Summer16_25nsV1_MC"},
        {"2017", "Fall17_V3_MC"},
        {"2018", "Autumn18_V7b_MC"},
        {"UL2017", "Summer19UL17_JRV2_MC"},
    };

    // jet mass resolution: https://twiki.cern.ch/twiki/bin/view/CMS/JetWtagging
    // nominal, up, down
    map<string, vector<float>> jmrValues = {
        {"2016", {1.0, 1.2, 0.8}},
        {"2017", {1.09, 1.14, 1.04}},
        // Use 2017 values for 2018 until 2018 are released
        {"2018", {1.09, 1.14, 1.04}},
        {"UL2017", {1.00, 1.00, 1.00}},  // placeholder
    };

    // jet mass scale
    // W-tagging PUPPI softdrop JMS values: https://twiki.cern.ch/twiki/bin/view/CMS/JetWtagging
    map<string, vector<float>> jmsValues = {
        {"2016", {1.00, 0.9906, 1.0094}},  // nominal, down, up
        {"2017", {0.982, 0.978, 0.986}},
        // Use 2017 values for 2018 until 2018 are released
        {"2018", {0.982, 0.978, 0.986}},
        {"UL2017", {1.000, 1.000, 1.000}}, // placeholder
    };
}