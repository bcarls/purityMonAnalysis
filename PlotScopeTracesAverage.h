

#ifndef PlotScopeTracesAverage_h
#define PlotScopeTracesAverage_h

#include "lifetimeCalcFit.h"
#include "lifetimeCalcLAPD.h"
#include "lifetimeCalc.h"


class PlotScopeTracesAverage{

  public:
    
    void RunPlotAndLifetime(TString PrMFile0, TString PrMFile1, TString PrMFile2, TString PrMFile3);

    lifetimeCalc calc;

  private:

    static const int nSamples = 5008;

};



#endif
