#include <TRandom.h>
int getRand01(){
    //randomly returns 0 or 1
    static long seed = 1;
    seed++;
    TRandom randGen = TRandom(seed);
    int rand = randGen.Binomial(1,0.5);
    return rand;   
}