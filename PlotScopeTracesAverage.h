

#ifndef PlotScopeTracesAverage_h
#define PlotScopeTracesAverage_h

#include "lifetimeCalcFit.h"
#include "lifetimeCalcLAPD.h"

// Number of samples
static const int nSamples = 5008;

class PlotScopeTracesAverage{

  public:
    
    void RunPlotAndLifetime(TString PrMFile0, TString PrMFile1, TString PrMFile2, TString PrMFile3);

    lifetimeCalcLAPD calc;

  private:


};



#endif
