
#ifndef PurityPlotMakerPublic_h
#define PurityPlotMakerPublic_h

class PurityPlotMakerPublic{

  public:
    void MakePlots();

    void SetPurityMonitor(int whichPurityMonitor){
      purityMonitor = whichPurityMonitor;
    }

    void AddDataFile(TString fileToAdd){
      listOfDataFiles.push_back(fileToAdd);
    }

  private: 

    std::vector<TString> listOfDataFiles;
    int purityMonitor; 

};

#endif

