#include <iostream>
#include "lifetimeCalcLAPD.h"
#include "TNtuple.h"




void lifetimeCalcLAPD::CalculateLifetime(TNtuple* oscillData, int IPrM, bool doNoiseSubtraction){

  // Determine lifetime, reinterpreted from the VB program
  int nEntries = (int)oscillData->GetEntries();
  float anoSignalVolt, catSignalVolt, anoNoiseVolt, catNoiseVolt;
  oscillData->SetBranchAddress("anoSignalSmooth",&anoSignalVolt);
  oscillData->SetBranchAddress("catSignalSmooth",&catSignalVolt);
  oscillData->SetBranchAddress("anoNoiseSmooth",&anoNoiseVolt);
  oscillData->SetBranchAddress("catNoiseSmooth",&catNoiseVolt);
  
  int TriggerTimeIndex = 500;
  double samplesPerSec = 2000000;
  if(IPrM == 1
    || IPrM == 2)
    samplesPerSec = 1000000;
  double secPerSample = 1/samplesPerSec;
  // Wait a little while after the trigger to look for the cathode min and anode max
  int CatTimeIndex = -1;
  double fCatTime = -1;
  double fCatSq = 0;
  int AnoTimeIndex = -1;
  double fAnoTime = -1;
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
  if(doNoiseSubtraction)
    for(int i = 0; i < 1000; i++){
      voltage = oscillData->GetEntry(i);
      if(catSignalVolt - catNoiseVolt < fCatPeak){
        fCatPeak = catSignalVolt-catNoiseVolt;
        CatTimeIndex = i;
      }
    }
  else
    for(int i = 0; i < 1000; i++){
      voltage = oscillData->GetEntry(i);
      if(catSignalVolt < fCatPeak){
        fCatPeak = catSignalVolt;
        CatTimeIndex = i;
      }
    }
  fCatTime = secPerSample*(-TriggerTimeIndex + CatTimeIndex);

  // Get the baseline 
  if(doNoiseSubtraction)
    for(int i = TriggerTimeIndex/2 - 25; i <= TriggerTimeIndex/2 + 24; i++){
      voltage = oscillData->GetEntry(i);
      fCatBase = fCatBase + catSignalVolt - catNoiseVolt;
    }
  else
    for(int i = TriggerTimeIndex/2 - 25; i <= TriggerTimeIndex/2 + 24; i++){
      voltage = oscillData->GetEntry(i);
      fCatBase = fCatBase + catSignalVolt;
    }
  fCatBase = fCatBase/50; 




  double maxCathNoiseOffBase = 0;
  if(doNoiseSubtraction)
    for(int i = TriggerTimeIndex/2 - 25; i <= TriggerTimeIndex/2 + 24; i++){
      voltage = oscillData->GetEntry(i);
      fCatSq = fCatSq + pow((fCatBase - catSignalVolt),2);
      if(maxCathNoiseOffBase < std::abs(fCatBase - catSignalVolt + catNoiseVolt))
        maxCathNoiseOffBase = std::abs(fCatBase - catSignalVolt + catNoiseVolt);
    }
  else 
    for(int i = TriggerTimeIndex/2 - 25; i <= TriggerTimeIndex/2 + 24; i++){
      voltage = oscillData->GetEntry(i);
      fCatSq = fCatSq + pow((fCatBase),2);
      if(maxCathNoiseOffBase < std::abs(fCatBase - catSignalVolt))
        maxCathNoiseOffBase = std::abs(fCatBase - catSignalVolt);
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
  if(doNoiseSubtraction)
    for(int i = 1000; i < nEntries; i++){
      voltage = oscillData->GetEntry(i);
      if(anoSignalVolt - anoNoiseVolt > fAnoPeak){
        fAnoPeak = anoSignalVolt - anoNoiseVolt;
        AnoTimeIndex = i;
      }
    }
  else
    for(int i = 1000; i < nEntries; i++){
      voltage = oscillData->GetEntry(i);
      if(anoSignalVolt > fAnoPeak){
        fAnoPeak = anoSignalVolt;
        AnoTimeIndex = i;
      }
    }

  fAnoTime = secPerSample*(-TriggerTimeIndex + AnoTimeIndex);



  int AnoBaselineIndexLow = 1602;
  int AnoBaselineIndexHigh = 3000;
  if(IPrM == 1
      || IPrM == 2){
    AnoBaselineIndexLow = 2500;
    AnoBaselineIndexHigh = 4200;
  }
  AnoBaselineIndexLow = (int)((double)(TriggerTimeIndex) + 0.666666*((double)AnoTimeIndex-(double)CatTimeIndex));
  AnoBaselineIndexHigh = (int)((double)(AnoTimeIndex) + 0.333333*((double)AnoTimeIndex-(double)CatTimeIndex));
  if(doNoiseSubtraction)
    for(int i = AnoBaselineIndexLow - 200; i <= AnoBaselineIndexLow + 199; i++){
      voltage = oscillData->GetEntry(i);
      // std::cout << i << " " << fAnoBase << std::endl;
      fAnoBase+=anoSignalVolt - anoNoiseVolt;
    }
  else
    for(int i = AnoBaselineIndexLow - 200; i <= AnoBaselineIndexLow + 199; i++){
      voltage = oscillData->GetEntry(i);
      // std::cout << i << " " << fAnoBase << std::endl;
      fAnoBase+=anoSignalVolt;
    }
  fAnoBase = fAnoBase/400;



  double maxAnoNoiseOffBase = 0;
  if(doNoiseSubtraction)
    for(int i = AnoBaselineIndexLow - 200; i <= AnoBaselineIndexLow + 199; i++){
      voltage = oscillData->GetEntry(i);
      fAnoSq = fAnoSq + pow((fAnoBase - anoSignalVolt + anoNoiseVolt),2);
      if(maxAnoNoiseOffBase < std::abs(fAnoBase - anoSignalVolt + anoNoiseVolt))
        maxAnoNoiseOffBase = std::abs(fAnoBase - anoSignalVolt + anoNoiseVolt);
    }
  else
    for(int i = AnoBaselineIndexLow - 200; i <= AnoBaselineIndexLow + 199; i++){
      voltage = oscillData->GetEntry(i);
      fAnoSq = fAnoSq + pow((fAnoBase - anoSignalVolt),2);
      if(maxAnoNoiseOffBase < std::abs(fAnoBase - anoSignalVolt))
        maxAnoNoiseOffBase = std::abs(fAnoBase - anoSignalVolt);
    }
  fAnoRMS= sqrt(fAnoSq/400);



  if(istop == 0){
    double a1 = fAnoBase + 0.25*(fAnoPeak-fAnoBase);
    double a2 = fAnoBase + 0.75*(fAnoPeak-fAnoBase);
    double da1 = 10000;
    double da2 = 10000;
    int AnoRiseStartIndex = TriggerTimeIndex + 0.75*(AnoTimeIndex-CatTimeIndex);
    if(doNoiseSubtraction)
      for(int i = AnoRiseStartIndex; i <= AnoTimeIndex; i++){
        voltage = oscillData->GetEntry(i);
        if(std::abs(anoSignalVolt - anoNoiseVolt - a1)<da1){
          da1 = std::abs(anoSignalVolt - anoNoiseVolt - a1);
          ta1 = -TriggerTimeIndex*secPerSample+i*secPerSample;
          va1 = anoSignalVolt - anoNoiseVolt;
        }
        if(std::abs(anoSignalVolt - anoNoiseVolt - a2)<da2){
          da2 = std::abs(anoSignalVolt - anoNoiseVolt - a2);
          ta2 = -TriggerTimeIndex*secPerSample+i*secPerSample;
          va2 = anoSignalVolt - anoNoiseVolt;
        }
      }
    else
      for(int i = AnoRiseStartIndex; i <= AnoTimeIndex; i++){
        voltage = oscillData->GetEntry(i);
        if(std::abs(anoSignalVolt - a1)<da1){
          da1 = std::abs(anoSignalVolt - a1);
          ta1 = -TriggerTimeIndex*secPerSample+i*secPerSample;
          va1 = anoSignalVolt;
        }
        if(std::abs(anoSignalVolt - a2)<da2){
          da2 = std::abs(anoSignalVolt - a2);
          ta2 = -TriggerTimeIndex*secPerSample+i*secPerSample;
          va2 = anoSignalVolt;
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
  fCatTrue = std::abs((fCatPeak - fCatBase)*fCathF);
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





