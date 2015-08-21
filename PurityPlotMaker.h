
#ifndef PurityPlotMaker_h
#define PurityPlotMaker_h

class PurityPlotMaker{

  public:
    void MakePlots();

    void AddDataFile(TString fileToAdd){
      listOfDataFiles.push_back(fileToAdd);
    }

  private: 

    std::vector<TString> listOfDataFiles;

};

#endif

