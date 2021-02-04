#include <ROOT/RVec.hxx>

using namespace ROOT::VecOps;
class ST_weight {
    public:

        ST_weight(float slope, float offset);
        ~ST_weight(){};

        RVec<float> eval(float ST);
    private:
        float a;
        float b;
};

ST_weight::ST_weight(float slope, float offset){
    a = slope;
    b = offset;

}


RVec<float> ST_weight::eval(float ST){
    RVec<float> weight(3);
    float nom = a*ST+b;
    weight[0] = nom;
    weight[1] = nom;
    weight[2] = nom;
    //no uncertainties on this one, but timber requires 3 values returned
    return weight;
};
