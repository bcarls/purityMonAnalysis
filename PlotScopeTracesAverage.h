

#ifndef PlotScopeTracesAverage_h
#define PlotScopeTracesAverage_h

#include "lifetimeCalcFit.h"
#include "lifetimeCalcLAPD.h"
#include "lifetimeCalc.h"


class PlotScopeTracesAverage{

  public:
    
    // void RunPlotAndLifetime(TString PrMFile0, TString PrMFile1, TString PrMFile2, TString PrMFile3, TString PrMFile4);
    void RunPlotAndLifetime();

    lifetimeCalc calc;

    void AddTraceFile(TString TraceFile){
      TraceFiles.push_back(TraceFile);
    };

    void ClearTraceFiles(){
      TraceFiles.clear();
    };

  private:

    static const int nSamples = 5008;
    
    std::vector<TString> TraceFiles;

};



#endif
