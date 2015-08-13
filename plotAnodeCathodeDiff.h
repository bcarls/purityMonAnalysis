

#ifndef plotAnodeCathodeDiff_h
#define plotAnodeCathodeDiff_h

#include "lifetimeCalcFit.h"

// Number of samples
static const int nSamples = 5008;

class plotAnodeCathodeDiff{

  public:
    
    void RunPlotAndLifetime(TString PrMFile);

    lifetimeCalcFit calc;

  private:


};



#endif
