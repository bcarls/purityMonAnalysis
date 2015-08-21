#include <iostream>
#include "lifetimeCalcLAPD.h"
#include "TNtuple.h"




void lifetimeCalcLAPD::CalculateLifetime(TNtuple* oscillData, int IPrM){

  // Determine lifetime, reinterpreted from the VB program
  int nEntries = (int)oscillData->GetEntries();
  float anoNoiseVolt0, anoSignalVolt0, catNoiseVolt0, catSignalVolt0;
  float anoNoiseVolt1, anoSignalVolt1, catNoiseVolt1, catSignalVolt1;
  float anoNoiseVolt2, anoSignalVolt2, catNoiseVolt2, catSignalVolt2;
  float anoNoiseVolt3, anoSignalVolt3, catNoiseVolt3, catSignalVolt3;
  oscillData->SetBranchAddress("anoNoiseSmooth0",&anoNoiseVolt0);
  oscillData->SetBranchAddress("anoSignalSmooth0",&anoSignalVolt0);
  oscillData->SetBranchAddress("catNoiseSmooth0",&catNoiseVolt0);
  oscillData->SetBranchAddress("catSignalSmooth0",&catSignalVolt0);
  oscillData->SetBranchAddress("anoNoiseSmooth1",&anoNoiseVolt1);
  oscillData->SetBranchAddress("anoSignalSmooth1",&anoSignalVolt1);
  oscillData->SetBranchAddress("catNoiseSmooth1",&catNoiseVolt1);
  oscillData->SetBranchAddress("catSignalSmooth1",&catSignalVolt1);
  oscillData->SetBranchAddress("anoNoiseSmooth2",&anoNoiseVolt2);
  oscillData->SetBranchAddress("anoSignalSmooth2",&anoSignalVolt2);
  oscillData->SetBranchAddress("catNoiseSmooth2",&catNoiseVolt2);
  oscillData->SetBranchAddress("catSignalSmooth2",&catSignalVolt2);
  oscillData->SetBranchAddress("anoNoiseSmooth3",&anoNoiseVolt3);
  oscillData->SetBranchAddress("anoSignalSmooth3",&anoSignalVolt3);
  oscillData->SetBranchAddress("catNoiseSmooth3",&catNoiseVolt3);
  oscillData->SetBranchAddress("catSignalSmooth3",&catSignalVolt3);
  
  // PrMs 1 and 4 are short and sample at 5000000 Hz
  // PrMs 0, 2, and 3 are long and sample at 2000000 Hz
  int TriggerTimeIndex = 500;
  double samplesPerSec = 2000000;
  if(IPrM == 1
      || IPrM == 2)
    samplesPerSec = 1000000;
  double secPerSample = 1/samplesPerSec;
  // Wait a little while after the trigger to look for the cathode min and anode max
  int CatTimeIndex = -1;
  double fCatTime = -1;
  double fCatBase = 0;
  double fCatSq = 0;
  int AnoTimeIndex = -1;
  double fAnoTime = -1;
  double fAnoBase = 0;
  double fAnoSq = 0;
  double RMSCut = 10;
  int istop = 0;
  double ta1 = 0;
  double va1 = 0;
  double ta2 = 0;
  double va2 = 0;
  int voltage;

  
  
  // Look for the cathode sigmal, it's now positive
  fCatPeak = 1000;
  for(int i = 0; i < 1000; i++){
    voltage = oscillData->GetEntry(i);
    double catSignalAverage = (catSignalVolt0+catSignalVolt1+catSignalVolt2+catSignalVolt3)/4;
    if(catSignalAverage < fCatPeak){
      fCatPeak = catSignalAverage ;
      CatTimeIndex = i;
    }
  }
  fCatTime = secPerSample*(-TriggerTimeIndex + CatTimeIndex);

  // Get the baseline 
  for(int i = TriggerTimeIndex/2 - 25; i <= TriggerTimeIndex/2 + 24; i++){
    voltage = oscillData->GetEntry(i);
    double catSignalAverage = (catSignalVolt0+catSignalVolt1+catSignalVolt2+catSignalVolt3)/4;
    fCatBase = fCatBase + (catSignalVolt0+catSignalVolt1+catSignalVolt2+catSignalVolt3)/4;
  }
  fCatBase = fCatBase/50; 




  double maxCathNoiseOffBase = 0;
  for(int i = TriggerTimeIndex/2 - 25; i <= TriggerTimeIndex/2 + 24; i++){
    voltage = oscillData->GetEntry(i);
    double catSignalAverage = (catSignalVolt0+catSignalVolt1+catSignalVolt2+catSignalVolt3)/4;
    fCatSq = fCatSq + pow((fCatBase - catSignalAverage),2);
    if(maxCathNoiseOffBase < fabs(fCatBase - catSignalAverage))
      maxCathNoiseOffBase = fabs(fCatBase - catSignalAverage);
  }
  fCatRMS = sqrt(fCatSq/50);

  std::cout << "Cathode Peak = " << fCatPeak << std::endl;
  std::cout << "Cathode Time = " << fCatTime << std::endl;
  std::cout << "Cathode Baseline = " << fCatBase << std::endl;
  std::cout << "Cathode RMS = " << fCatRMS << std::endl;
  std::cout << "Cathode Max Noise Off Baseline = " << maxCathNoiseOffBase << std::endl;



  // Now do anode
  fAnoPeak = -1000;
  AnoTimeIndex = -1;
  // Start at sample 1500 to get a fair bit past the cathode signal
  for(int i = 1500; i < nEntries; i++){
    voltage = oscillData->GetEntry(i);
    double anoSignalAverage = (anoSignalVolt0+anoSignalVolt1+anoSignalVolt2+anoSignalVolt3)/4;
    if(anoSignalAverage  > fAnoPeak){
      fAnoPeak = anoSignalAverage;
      AnoTimeIndex = i;
    }
  }

  fAnoTime = secPerSample*(-TriggerTimeIndex + AnoTimeIndex);

  // Find the anode baseline
  // int AnoBaselineIndexLow = 1602
  // int AnoBaselineIndexHigh = 3000;
  // if(IPrM == 1
  //     || IPrM == 2){
  //   AnoBaselineIndexLow = 2500;
  //   AnoBaselineIndexHigh = 4200;
  // }
  // int AnoBaselineIndexLow = (int)((double)(TriggerTimeIndex) + 0.666666*((double)AnoTimeIndex-(double)CatTimeIndex));
  // int AnoBaselineIndexHigh = (int)((double)(AnoTimeIndex) + 0.333333*((double)AnoTimeIndex-(double)CatTimeIndex));
  // // std::cout << AnoTimeIndex << " " << CatTimeIndex << std::endl;
  // // std::cout << AnoBaselineIndexLow  << " " << AnoBaselineIndexHigh << std::endl;
  // for(int i = AnoBaselineIndexLow - 25; i <= AnoBaselineIndexLow + 24; i++){
  //   voltage = oscillData->GetEntry(i);
  //   // std::cout << i << " " << fAnoBase << std::endl;
  //   fAnoBase+=anoSignalVolt-catSignalVolt;
  // }
  // for(int i = AnoBaselineIndexHigh - 25; i <= AnoBaselineIndexHigh + 24; i++){
  //   voltage = oscillData->GetEntry(i);
  //   // std::cout << i << " " << fAnoBase << std::endl;
  //   fAnoBase+=anoSignalVolt-catSignalVolt;
  // }
  // fAnoBase = fAnoBase/100;


  int AnoBaselineIndexLow = 1602;
  int AnoBaselineIndexHigh = 3000;
  if(IPrM == 1
      || IPrM == 2){
    AnoBaselineIndexLow = 2500;
    AnoBaselineIndexHigh = 4200;
  }
  AnoBaselineIndexLow = (int)((double)(TriggerTimeIndex) + 0.666666*((double)AnoTimeIndex-(double)CatTimeIndex));
  AnoBaselineIndexHigh = (int)((double)(AnoTimeIndex) + 0.333333*((double)AnoTimeIndex-(double)CatTimeIndex));
  // std::cout << AnoTimeIndex << " " << CatTimeIndex << std::endl;
  // std::cout << AnoBaselineIndexLow  << " " << AnoBaselineIndexHigh << std::endl;
  for(int i = AnoBaselineIndexLow - 50; i <= AnoBaselineIndexLow + 49; i++){
    double anoSignalAverage = (anoSignalVolt0+anoSignalVolt1+anoSignalVolt2+anoSignalVolt3)/4;
    voltage = oscillData->GetEntry(i);
    // std::cout << i << " " << fAnoBase << std::endl;
    fAnoBase+=anoSignalAverage;
  }
  fAnoBase = fAnoBase/100;






  double maxAnoNoiseOffBase = 0;
  for(int i = AnoBaselineIndexLow - 200; i <= AnoBaselineIndexLow + 199; i++){
    double anoSignalAverage = (anoSignalVolt0+anoSignalVolt1+anoSignalVolt2+anoSignalVolt3)/4;
    voltage = oscillData->GetEntry(i);
    fAnoSq = fAnoSq + pow((fAnoBase - anoSignalAverage),2);
    if(maxAnoNoiseOffBase < fabs(fAnoBase - anoSignalAverage))
      maxAnoNoiseOffBase = fabs(fAnoBase - anoSignalAverage);
  }
  fAnoRMS= sqrt(fAnoSq/400);



  if(istop == 0){
    double a1 = fAnoBase + 0.25*(fAnoPeak-fAnoBase);
    double a2 = fAnoBase + 0.75*(fAnoPeak-fAnoBase);
    double da1 = 10000;
    double da2 = 10000;
    int AnoRiseStartIndex = TriggerTimeIndex + 0.75*(AnoTimeIndex-CatTimeIndex);
    // for(int i = 1602; i <= AnoTimeIndex; i++){
    // for(int i = 3000; i <= AnoTimeIndex; i++){
    for(int i = AnoRiseStartIndex; i <= AnoTimeIndex; i++){
      voltage = oscillData->GetEntry(i);
      double anoSignalAverage = (anoSignalVolt0+anoSignalVolt1+anoSignalVolt2+anoSignalVolt3)/4;
      if(fabs(anoSignalAverage - a1)<da1){
        da1 = fabs(anoSignalAverage- a1);
        ta1 = -TriggerTimeIndex*secPerSample+i*secPerSample;
        va1 = anoSignalAverage;
      }
      if(fabs(anoSignalAverage - a2)<da2){
        da2 = fabs(anoSignalAverage - a2);
        ta2 = -TriggerTimeIndex*secPerSample+i*secPerSample;
        va2 = anoSignalAverage;
      }

    }
    fAnoRise = (ta2-ta1)*(fAnoPeak-fAnoBase)/(va2-va1);
  }

  std::cout << "Anode Peak = " << fAnoPeak << std::endl;
  std::cout << "Anode Time = " << fAnoTime << std::endl;
  std::cout << "Anode Baseline = " << fAnoBase << std::endl;
  std::cout << "Anode Rise = " << fAnoRise << std::endl;
  std::cout << "Anode RMS = " << fAnoRMS << std::endl;
  // std::cout << "Anode Max Noise Off Baseline = " << maxAnoNoiseOffBase << std::endl;

  double RC = 0.000119;
  
  double gAlphaGBeta = 1;


  fCathF = (fCatTime + 0.000006) / (RC*(1-exp(-(fCatTime + 0.000006)/RC)));
  fAnoF = (fAnoRise + 0.000005)/(RC*(1-exp(-(fAnoRise + 0.000005)/RC)));
  // fAnoF = 1.283;
  // fCathF = 1.627;
  fAnoTrue = (fAnoPeak - fAnoBase)*fAnoF;
  fCatTrue = fabs((fCatPeak - fCatBase)*fCathF);
  fLifeTime = 0;
  fImpurities = 99999;
  if(fAnoTrue > 0){
    if(fCatTrue/fAnoTrue>0){
      fLifeTime = fAnoTime/(log( gAlphaGBeta*fCatTrue/fAnoTrue));
      fImpurities = 0.00015/fLifeTime;
    }
    else
      fLifeTime = 0;
  }
 
  if(fLifeTime > 0.10 
      || fLifeTime < 0)
    fLifeTime = 0.10;





  std::cout << "Cath Factor = " << fCathF << std::endl;
  std::cout << "Anode Factor = " << fAnoF << std::endl;
  std::cout << "Anode True = " << fAnoTrue << std::endl;
  std::cout << "Cathode True = " << fCatTrue << std::endl;
  std::cout << "LifeTime = " << fLifeTime << std::endl;
  std::cout << "Impurities = " << fImpurities << std::endl;
  
  return;
}





