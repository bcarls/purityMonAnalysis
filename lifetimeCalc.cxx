#include <iostream>
#include "lifetimeCalc.h"
#include "TNtuple.h"




void lifetimeCalc::CalculateLifetime(TNtuple* oscillData, int IPrM){

  // Determine lifetime, reinterpreted from the VB program
  int nEntries = (int)oscillData->GetEntries();
  float anoNoiseVolt, anoSignalVolt, catNoiseVolt, catSignalVolt;
  oscillData->SetBranchAddress("anoNoiseSmooth",&anoNoiseVolt);
  oscillData->SetBranchAddress("anoSignalSmooth",&anoSignalVolt);
  oscillData->SetBranchAddress("catNoiseSmooth",&catNoiseVolt);
  oscillData->SetBranchAddress("catSignalSmooth",&catSignalVolt);
  //oscillData->SetBranchAddress("anoNoise",&anoNoiseVolt);
  //oscillData->SetBranchAddress("anoSignal",&anoSignalVolt);
  //oscillData->SetBranchAddress("catNoise",&catNoiseVolt);
  //oscillData->SetBranchAddress("catSignal",&catSignalVolt);
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
  double fCatRMS = 0;
  int AnoTimeIndex = -1;
  double fAnoTime = -1;
  double fAnoBase = 0;
  double fAnoSq = 0;
  double fAnoRMS = 0;
  double RMSCut = 10;
  int istop = 0;
  double ta1 = 0;
  double va1 = 0;
  double ta2 = 0;
  double va2 = 0;
  int voltage;

  
  
  // Look for the cathode sigmal, it's now positive
  fCatPeak = -1000;
  for(int i = 0; i < 1000; i++){
    voltage = oscillData->GetEntry(i);
    if(anoSignalVolt-catSignalVolt > fCatPeak){
      fCatPeak = anoSignalVolt-catSignalVolt;
      CatTimeIndex = i;
    }
  }
  fCatTime = secPerSample*(-TriggerTimeIndex + CatTimeIndex);

  // Get the baseline 
  for(int i = 3*TriggerTimeIndex/4 - 25; i <= 3*TriggerTimeIndex/4 + 24; i++){
    voltage = oscillData->GetEntry(i);
    fCatBase = fCatBase + anoSignalVolt-catSignalVolt;
  }
  fCatBase = fCatBase/50;
  double maxCathNoiseOffBase = 0;
  for(int i = TriggerTimeIndex/2 - 25; i <= TriggerTimeIndex/2 + 24; i++){
    voltage = oscillData->GetEntry(i);
    fCatSq = fCatSq + pow((fCatBase - anoSignalVolt + catSignalVolt),2);
    if(maxCathNoiseOffBase < fabs(fCatBase - anoSignalVolt + catSignalVolt))
      maxCathNoiseOffBase = fabs(fCatBase - anoSignalVolt + catSignalVolt);
  }
  fCatRMS = sqrt(fCatSq/50);
  std::cout << "Cathode Peak = " << fCatPeak << std::endl;
  std::cout << "Cathode Time = " << fCatTime << std::endl;
  std::cout << "Cathode Baseline = " << fCatBase << std::endl;
  std::cout << "Cathode RMS = " << fCatRMS << std::endl;
  std::cout << "Cathode Max Noise Off Baseline = " << maxCathNoiseOffBase << std::endl;
  if( fabs(fCatPeak - fCatBase) < RMSCut*fCatRMS)
    fCatPeak = fCatBase;



  // Now do anode
  fAnoPeak = -1000;
  AnoTimeIndex = -1;
  // Start at sample 1500 to get a fair bit past the cathode signal
  for(int i = 1500; i < nEntries; i++){
    voltage = oscillData->GetEntry(i);
    if(anoSignalVolt-catSignalVolt > fAnoPeak){
      fAnoPeak = anoSignalVolt-catSignalVolt;
      AnoTimeIndex = i;
    }
  }


  // Find the anode baseline
  // int AnoBaselineIndexLow = 1602
  // int AnoBaselineIndexHigh = 3000;
  // if(IPrM == 1
  //     || IPrM == 2){
  //   AnoBaselineIndexLow = 2500;
  //   AnoBaselineIndexHigh = 4200;
  // }
  int AnoBaselineIndexLow = TriggerTimeIndex + 0.666666*(AnoTimeIndex-CatTimeIndex);
  int AnoBaselineIndexHigh = AnoTimeIndex + 0.333333*(AnoTimeIndex-CatTimeIndex);
  std::cout << AnoBaselineIndexLow  << " " << AnoBaselineIndexHigh << std::endl;
  for(int i = AnoBaselineIndexLow - 25; i <= AnoBaselineIndexLow + 24; i++){
    voltage = oscillData->GetEntry(i);
    fAnoBase = + anoSignalVolt-catSignalVolt;
  }
  for(int i = AnoBaselineIndexHigh - 25; i <= AnoBaselineIndexHigh + 24; i++){
    voltage = oscillData->GetEntry(i);
    fAnoBase = + anoSignalVolt-catSignalVolt;
  }
  fAnoBase = fAnoBase/100;



  double maxAnoNoiseOffBase = 0;
  for(int i = AnoBaselineIndexLow - 25; i <= AnoBaselineIndexLow + 24; i++){
    voltage = oscillData->GetEntry(i);
    fAnoSq = fAnoSq + pow((fAnoBase - anoSignalVolt + catSignalVolt),2);
    if(maxAnoNoiseOffBase < fabs(fAnoBase - anoSignalVolt + catSignalVolt))
      maxAnoNoiseOffBase = fabs(fAnoBase - anoSignalVolt + catSignalVolt);
  }
  for(int i = AnoBaselineIndexHigh - 25; i <= AnoBaselineIndexHigh + 24; i++){
    voltage = oscillData->GetEntry(i);
    fAnoSq = fAnoSq + pow((fAnoBase - anoSignalVolt + catSignalVolt),2);
    if(maxAnoNoiseOffBase < fabs(fAnoBase - anoSignalVolt + catSignalVolt))
      maxAnoNoiseOffBase = fabs(fAnoBase - anoSignalVolt + catSignalVolt);
  }
  fAnoRMS= sqrt(fAnoSq/100);








  fAnoTime = secPerSample*(-TriggerTimeIndex + AnoTimeIndex);

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
      if(fabs(anoSignalVolt-catSignalVolt - a1)<da1){
        da1 = fabs(anoSignalVolt-catSignalVolt - a1);
        ta1 = -TriggerTimeIndex*secPerSample+i*secPerSample;
        va1 = anoSignalVolt-catSignalVolt;
      }
      if(fabs(anoSignalVolt-catSignalVolt - a2)<da2){
        da2 = fabs(anoSignalVolt-catSignalVolt - a2);
        ta2 = -TriggerTimeIndex*secPerSample+i*secPerSample;
        va2 = anoSignalVolt-catSignalVolt;
      }

    }
    fAnoRise = (ta2-ta1)*(fAnoPeak-fAnoBase)/(va2-va1);
  }

  std::cout << "Anode Peak = " << fAnoPeak << std::endl;
  std::cout << "Anode Time = " << fAnoTime << std::endl;
  std::cout << "Anode Baseline = " << fAnoBase << std::endl;
  std::cout << "Anode Rise = " << fAnoRise << std::endl;
  std::cout << "Anode RMS = " << fAnoRMS << std::endl;
  std::cout << "Anode Max Noise Off Baseline = " << maxAnoNoiseOffBase << std::endl;

  double RC = 0.000119;
  
  double gAlphaGBeta = 1;


  fCathF = (fCatTime + 0.000006) / (RC*(1-exp(-(fCatTime + 0.000006)/RC)));
  fAnoF = (fAnoRise + 0.000005)/(RC*(1-exp(-(fAnoRise + 0.000005)/RC)));
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


  double fLifeTimeNoiseShiftDown = 0;
  if(fAnoTrue > 0){
    if(fCatTrue/fAnoTrue>0){
      fLifeTimeNoiseShiftDown = fAnoTime/(log( gAlphaGBeta*(fCatTrue-maxCathNoiseOffBase)/(fAnoTrue-maxAnoNoiseOffBase)));
    }
    else
      fLifeTimeNoiseShiftDown = 0;
  }
 
  if(fLifeTimeNoiseShiftDown > 0.10 
      || fLifeTimeNoiseShiftDown < 0)
    fLifeTimeNoiseShiftDown = 0.10;


  double fLifeTimeNoiseDifference = fabs(fLifeTime-fLifeTimeNoiseShiftDown);



  std::cout << "Cath Factor = " << fCathF << std::endl;
  std::cout << "Anode Factor = " << fAnoF << std::endl;
  std::cout << "Anode True = " << fAnoTrue << std::endl;
  std::cout << "Cathode True = " << fCatTrue << std::endl;
  std::cout << "LifeTime = " << fLifeTime << std::endl;
  std::cout << "LifeTime Noise Shift Down = " << fLifeTimeNoiseShiftDown << std::endl;
  std::cout << "LifeTime Noise Difference = " << fLifeTimeNoiseDifference << std::endl;
  std::cout << "Impurities = " << fImpurities << std::endl;
  
  return;
}





