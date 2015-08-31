

#ifndef PlotScopeTraces_h
#define PlotScopeTraces_h

#include "lifetimeCalcLAPD.h"

class PlotScopeTraces{

  public:
    
    void RunPlotAndLifetime(TString PrMFile);

    lifetimeCalcLAPD calc;

  private:

    // Number of samples
    static const int nSamples = 5008;


};



#endif
