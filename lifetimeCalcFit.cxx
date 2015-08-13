#include <iostream>
#include "lifetimeCalcFit.h"
#include "TNtuple.h"

double lifetimeCalcFit::SinusoidReject(double *x, double *par){

  if( x[0] > 3 && x[0] < 4.5 ){
    TF1::RejectPoint();
    return 0;
  }
  return par[0]*cos(x[0]*par[1]+par[2])+par[3];

}


double lifetimeCalcFit::SignalFunction(double* x, double *par){

  if( x[0] > 3.25 && x[0] < 5){
    return par[0]*(par[2]+par[3]*(x[0]-par[1])*0.5*(1-TMath::Sign(1,(x[0]-par[1]))))*exp(-(x[0]-par[1])/0.119) + par[4]*cos(x[0]*par[5]+par[6]) + par[7];
  }

  if( x[0] < 3.25 || x[0] > 5){
    return par[4]*cos(x[0]*par[5]+par[6]) + par[7];
  }

  TF1::RejectPoint();
  return 0;

}




void lifetimeCalcFit::CalculateLifetime(TNtuple* oscillData, int IPrM){

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
  std::cout << fCatBase << std::endl;
  fCatBase = fCatBase/50;
  std::cout << fCatBase << std::endl;
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

  fAnoTime = secPerSample*(-TriggerTimeIndex + AnoTimeIndex);

  // Find the anode baseline
  // int AnoBaselineIndexLow = 1602
  // int AnoBaselineIndexHigh = 3000;
  // if(IPrM == 1
  //     || IPrM == 2){
  //   AnoBaselineIndexLow = 2500;
  //   AnoBaselineIndexHigh = 4200;
  // }
  int AnoBaselineIndexLow = (int)((double)(TriggerTimeIndex) + 0.666666*((double)AnoTimeIndex-(double)CatTimeIndex));
  int AnoBaselineIndexHigh = (int)((double)(AnoTimeIndex) + 0.333333*((double)AnoTimeIndex-(double)CatTimeIndex));
  // std::cout << AnoTimeIndex << " " << CatTimeIndex << std::endl;
  // std::cout << AnoBaselineIndexLow  << " " << AnoBaselineIndexHigh << std::endl;
  for(int i = AnoBaselineIndexLow - 25; i <= AnoBaselineIndexLow + 24; i++){
    voltage = oscillData->GetEntry(i);
    // std::cout << i << " " << fAnoBase << std::endl;
    fAnoBase+=anoSignalVolt-catSignalVolt;
  }
  for(int i = AnoBaselineIndexHigh - 25; i <= AnoBaselineIndexHigh + 24; i++){
    voltage = oscillData->GetEntry(i);
    // std::cout << i << " " << fAnoBase << std::endl;
    fAnoBase+=anoSignalVolt-catSignalVolt;
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
 



  //Find minimum and maximum voltages, needed to get range right for plotting
  // float anoNoiseVolt, anoSignalVolt, catNoiseVolt, catSignalVolt;
  float minVoltage=999999, maxVoltage=-999999;
  oscillData->SetBranchAddress("anoNoiseSmooth",&anoNoiseVolt);
  oscillData->SetBranchAddress("anoSignalSmooth",&anoSignalVolt);
  oscillData->SetBranchAddress("catNoiseSmooth",&catNoiseVolt);
  oscillData->SetBranchAddress("catSignalSmooth",&catSignalVolt);
  for(int i = 0; i < nEntries; i++){
    voltage = oscillData->GetEntry(i);
    if(anoSignalVolt - catSignalVolt < minVoltage)
       minVoltage=anoSignalVolt-catSignalVolt;
    if(anoSignalVolt - catSignalVolt > maxVoltage)
       maxVoltage=anoSignalVolt-catSignalVolt;

  }









  TCanvas *c1 = new TCanvas("c1","Tree test");
  TH2F *frame = new TH2F("frame","", 100, 1000*(oscillData->GetMinimum("time")), 1000*(oscillData->GetMaximum("time")), 100, 1000*(minVoltage-0.1*fabs(maxVoltage-minVoltage)), 1000*(maxVoltage+0.5*fabs(maxVoltage-minVoltage)));
  gStyle->SetOptStat(0);
  frame->Draw();
  TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);
  leg->SetFillColor(0);
  oscillData->SetMarkerStyle(2);
  oscillData->SetMarkerColor(kOrange+8);
  oscillData->SetLineColor(kOrange+8);
  oscillData->SetLineWidth(4);
  oscillData->SetLineStyle(2);
  oscillData->SetMarkerSize(0.5);
  oscillData->Draw("1000*(anoSignalSmooth):1000*time","","LSAME");
  // oscillData->Draw("1000*(anoSignalSmooth-anoNoiseSmooth-catSignalSmooth+catNoiseSmooth):1000*time","","LSAME");
  // oscillData->Draw("1000*(anoSignal-catSignal):1000*time","","LSAME");
  TGraph *graph = (TGraph*)gPad->GetPrimitive("Graph");
  frame->GetXaxis()->SetTitle("ms");
  frame->GetYaxis()->SetTitle("mV");
  leg->AddEntry(graph,"anode signal","l");
  leg->Draw();

  // Old stuff, didn't really work
  // TF1 *f1 = new TF1("f1","[0]*TMath::Erf((x-[1])/[2])*exp(-x/[3])",-10,10);
  // TF1 *f1 = new TF1("f1","[0]*TMath::Erf(10*(x-[1]))*exp(-x/0.119)",0,1);
  // TF1 *f1 = new TF1("f1","[0]*0.5*(1+TMath::Sign(1,(x-[1])))*TMath::Erf([2]*(x-[1]))*exp(-(x-[1])/0.119)+[3]*exp(-0.5*((x-[1])/[4]^2))",3.1,3.9);

  //This worked well!
  // TF1 *f1 = new TF1("f1","[0]*([2]+[3]*(x-[1])*0.5*(1-TMath::Sign(1,(x-[1]))))*exp(-(x-[1])/0.119)+[4]",3.25,3.6);
  // f1->SetParameter(0,1);
  // f1->SetParameter(1,3.32);
  // f1->SetParameter(2,10);
  // f1->SetParameter(3,1);
  // f1->SetParameter(4,0.1);


 
  // Fit background with cos?
  // TF1 *f1 = new TF1("f1","[0]*cos(x*[1]+[2])+[3]",0,3);
  TF1 *f1 = new TF1("f1",SinusoidReject,0,5,4);
  f1->SetParameter(0,10);
  // f1->SetParLimits(0,0,1000000);
  // f1->SetParameter(1,3.3);
  f1->FixParameter(1,1.12);
  // f1->SetParLimits(1,3,3.5);
  f1->SetParameter(2,0);
  f1->SetParameter(3,0);

  graph->Fit("f1","R");


  // The complete fit
  // TF1 *f2 = new TF1("f2","[0]*([2]+[3]*(x-[1])*0.5*(1-TMath::Sign(1,(x-[1]))))*exp(-(x-[1])/0.119) + [4]*cos(x*[5]+[6]) + [7]",0,5);
  TF1 *f2 = new TF1("f2",SignalFunction,0,5,8);
  f2->SetParameter(0,0.1);
  f2->FixParameter(1,3.32);
  // f2->SetParameter(1,6);
  f2->SetParameter(2,30);
  f2->SetParameter(3,20);
  f2->FixParameter(4,f1->GetParameter(0));
  f2->FixParameter(5,f1->GetParameter(1));
  f2->FixParameter(6,f1->GetParameter(2));
  f2->FixParameter(7,f1->GetParameter(3));


  
  // f1->Draw("SAME");
  graph->Fit("f2","R");


//   double AnodePeakX = 3.32;
//   double AnodeFitPeak = [0]*([2]+[3]*(AnodePeakX-[1])*0.5*(1-TMath::Sign(1,(x-[1]))))*exp(-(x-[1])/0.119);


  return;
}





