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
		//pt cats 350-400, 400-500, 500+
		//Provided by Loukas, WP 0.94
		float SF2016_T[3][3] = {{1.137,1.102,1.174},{1.159,1.130,1.189},{1.096,1.065,1.129}};
		float SF2017_T[3][3] = {{1.091,1.048,1.136},{1.061,1.031,1.093},{1.057,1.033,1.082}};
		float SF2018_T[3][3] = {{1.091,1.048,1.136},{1.061,1.031,1.093},{1.057,1.033,1.082}};
		//Provided by Loukas, WP 0.90
		float SF2016_L[3][3] = {{1.120,1.084,1.157},{1.152,1.122,1.182},{1.086,1.054,1.120}};
		float SF2017_L[3][3] = {{1.105,1.062,1.149},{1.065,1.036,1.097},{1.049,1.025,1.073}};
		float SF2018_L[3][3] = {{1.105,1.062,1.149},{1.065,1.036,1.097},{1.049,1.025,1.073}};
};

