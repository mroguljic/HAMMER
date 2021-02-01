#include <ROOT/RVec.hxx>
#include <TRandom.h>

using namespace ROOT::VecOps;
using rvec_i =  RVec<int>;
using rvec_f =  RVec<float>;

class btagSFHandler {
    public:
		int _nWps;
		rvec_f _wps;
		rvec_f _effs;
		int _year;
		int _var;
		TRandom _randGen;

        btagSFHandler(rvec_f wps,rvec_f effs,int year,int var);
        ~btagSFHandler(){};
        int createTaggingCategories(float taggerVal);
		int updateTaggingCategories(int jetCat,float jetPt);
		int bothLessThanOne(int jetCat, float sfT, float sfL);
		int bothGreaterThanOne(int jetCat, float sfT, float sfL);
		float getSF(float jetPt,int wpCat,int year);


		//SF[i][j] i denotes nom/down/up, j denotes pt category
		float SF2016[3][3] = {{0.90,1.20,0.90},{0.58,0.90,0.63},{1.23,1.52,1.23}};
		float SF2017[3][3] = {{1.13,0.95,1.00},{0.68,0.51,0.50},{1.58,1.35,1.50}};
		float SF2018[3][3] = {{1.00,1.00,1.00},{0.50,0.50,0.50},{1.50,1.50,1.50}};
};

