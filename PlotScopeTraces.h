

#ifndef PlotScopeTraces_h
#define PlotScopeTraces_h

#include "lifetimeCalcLAPD.h"

// Number of samples
static const int nSamples = 5008;

class PlotScopeTraces{

  public:
    
    void RunPlotAndLifetime(TString PrMFile);

    lifetimeCalcLAPD calc;

  private:


};



#endif
