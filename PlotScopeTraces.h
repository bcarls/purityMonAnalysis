

#ifndef PlotScopeTraces_h
#define PlotScopeTraces_h

#include "lifetimeCalc.h"

class PlotScopeTraces{

  public:
    
    void RunPlotAndLifetime(TString PrMFile);

    lifetimeCalc calc;

  private:

    // Number of samples
    static const int nSamples = 5008;

};



#endif
