

#ifndef plotAnodeCathodeDiffAverage_h
#define plotAnodeCathodeDiffAverage_h

#include "lifetimeCalcFit.h"
#include "lifetimeCalcLAPD.h"

// Number of samples
static const int nSamples = 5008;

class plotAnodeCathodeDiffAverage{

  public:
    
    void RunPlotAndLifetime(TString PrMFile0, TString PrMFile1, TString PrMFile2, TString PrMFile3);

    lifetimeCalcLAPD calc;

  private:


};



#endif
