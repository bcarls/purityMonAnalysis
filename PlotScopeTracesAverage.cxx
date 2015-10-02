
#include "PlotScopeTracesAverage.h"
#include "TString.h"

// Plot the data coming from the digitizer card reading the purity monitors



void PlotScopeTracesAverage::RunPlotAndLifetime(){
  TNtuple *oscillData = new TNtuple("oscillData","NTUPLE","time:anoNoise0");

  TString sPrM = TraceFiles[0][23];
  TString runNumber = TraceFiles[0](15,6);

  // Subtract noise traces from signal?
  bool doNoiseSubtraction = false;
  if(atoi(runNumber.Data()) >= 1816 && atoi(sPrM.Data()) == 1)
    doNoiseSubtraction = true;
  if(atoi(sPrM.Data()) == 0)
    doNoiseSubtraction = true;

  // Select file to plot data from
  FILE *fp = fopen(TraceFiles[0].Data(),"r");
  float time, voltage;
  int month, day, year, hour, minute, second;
  char AMorPM[2];
  int n = 0;
  int ISmooth = 40;
  // Definte ncols and skip first line
  // int ncols = fscanf(fp,"%d/%d/%d %d:%d:%d %c%c ",&month,&day,&year,&hour,&minute,&second,&AMorPM[0],&AMorPM[1]);
  int ncols = fscanf(fp,"%d/%d/%d %d:%d:%d %c%c  Pass = 0",&month,&day,&year,&hour,&minute,&second,&AMorPM[0],&AMorPM[1]);
  std::cout << month << "/" << day << "/" << year <<" " << 
    hour << ":" << minute << ":" << second << " " << AMorPM[0] << AMorPM[1] << std::endl;
  TString pThingString = AMorPM[0];
  if(pThingString == "P" && hour < 12)
    hour+=12;
  if(pThingString == "A" && hour == 12)
    hour-=12;
  TDatime datime(year,month,day,hour,minute,second);
  datime.Print();
  // Read in time and anode noise
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    if (n < 5)
      std::cout << time << " " << voltage << std::endl;
    oscillData->Fill(time,voltage);
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise
  n=0;
  TBranch *catNoise0 = oscillData->Branch("catNoise0", &voltage, "catNoise0/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    if (n < 5)
      std::cout << time << " " << voltage << std::endl;
    catNoise0->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal
  n=0;
  TBranch *anoSignal0 = oscillData->Branch("anoSignal0", &voltage, "anoSignal0/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    if (n < 5)
      std::cout << time << " " << voltage << std::endl;
    anoSignal0->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal
  n=0;
  TBranch *catSignal0 = oscillData->Branch("catSignal0", &voltage, "catSignal0/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    if (n < 5)
      std::cout << time << " " << voltage << std::endl;
    catSignal0->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode noise, smoothed
  n=0;
  TBranch *anoNoiseSmooth0 = oscillData->Branch("anoNoiseSmooth0", &voltage, "anoNoiseSmooth0/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoNoiseSmooth0->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise, smoothed
  n=0;
  TBranch *catNoiseSmooth0 = oscillData->Branch("catNoiseSmooth0", &voltage, "catNoiseSmooth0/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catNoiseSmooth0->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal, smoothed
  n=0;
  TBranch *anoSignalSmooth0 = oscillData->Branch("anoSignalSmooth0", &voltage, "anoSignalSmooth0/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoSignalSmooth0->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal, smooth
  n=0;
  TBranch *catSignalSmooth0 = oscillData->Branch("catSignalSmooth0", &voltage, "catSignalSmooth0/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catSignalSmooth0->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }



  // Select file to plot data from
  FILE *fp1 = fopen(TraceFiles[1].Data(),"r");
  // float time, voltage;
  // int month, day, year, hour, minute, second
  // char AMorPM[2];
  // int n = 0;
  // Definte ncols and skip first line
  ncols = fscanf(fp1,"%d/%d/%d %d:%d:%d %c%c  Pass = 0",&month,&day,&year,&hour,&minute,&second,&AMorPM[0],&AMorPM[1]);
  std::cout << month << "/" << day << "/" << year <<" " << 
    hour << ":" << minute << ":" << second << " " << AMorPM[0] << AMorPM[1] << std::endl;
  pThingString = AMorPM[0];
  if(pThingString == "P" && hour < 12)
    hour+=12;
  TDatime datime1(year,month,day,hour,minute,second);
  datime1.Print();
  // Read in time and anode noise
  n=0;
  TBranch *anoNoise1 = oscillData->Branch("anoNoise1", &voltage, "anoNoise1/F");
  while (1) {
    ncols = fscanf(fp1,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoNoise1->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise
  n=0;
  TBranch *catNoise1 = oscillData->Branch("catNoise1", &voltage, "catNoise1/F");
  while (1) {
    ncols = fscanf(fp1,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catNoise1->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal
  n=0;
  TBranch *anoSignal1 = oscillData->Branch("anoSignal1", &voltage, "anoSignal1/F");
  while (1) {
    ncols = fscanf(fp1,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoSignal1->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal
  n=0;
  TBranch *catSignal1 = oscillData->Branch("catSignal1", &voltage, "catSignal1/F");
  while (1) {
    ncols = fscanf(fp1,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catSignal1->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode noise, smoothed
  n=0;
  TBranch *anoNoiseSmooth1 = oscillData->Branch("anoNoiseSmooth1", &voltage, "anoNoiseSmooth1/F");
  while (1) {
    ncols = fscanf(fp1,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoNoiseSmooth1->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise, smoothed
  n=0;
  TBranch *catNoiseSmooth1 = oscillData->Branch("catNoiseSmooth1", &voltage, "catNoiseSmooth1/F");
  while (1) {
    ncols = fscanf(fp1,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catNoiseSmooth1->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal, smoothed
  n=0;
  TBranch *anoSignalSmooth1 = oscillData->Branch("anoSignalSmooth1", &voltage, "anoSignalSmooth1/F");
  while (1) {
    ncols = fscanf(fp1,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoSignalSmooth1->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal, smooth
  n=0;
  TBranch *catSignalSmooth1 = oscillData->Branch("catSignalSmooth1", &voltage, "catSignalSmooth1/F");
  while (1) {
    ncols = fscanf(fp1,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catSignalSmooth1->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }






  // Select file to plot data from
  FILE *fp2 = fopen(TraceFiles[2].Data(),"r");
  // float time, voltage;
  // int month, day, year, hour, minute, second;
  // char AMorPM[2];
  // int n = 0;
  // Definte ncols and skip first line
  ncols = fscanf(fp2,"%d/%d/%d %d:%d:%d %c%c  Pass = 0",&month,&day,&year,&hour,&minute,&second,&AMorPM[0],&AMorPM[1]);
  std::cout << month << "/" << day << "/" << year <<" " << 
    hour << ":" << minute << ":" << second << " " << AMorPM[0] << AMorPM[1] << std::endl;
  pThingString = AMorPM[0];
  if(pThingString == "P" && hour < 12)
    hour+=12;
  TDatime datime2(year,month,day,hour,minute,second);
  datime2.Print();
  // Read in time and anode noise
  n=0;
  TBranch *anoNoise2 = oscillData->Branch("anoNoise2", &voltage, "anoNoise2/F");
  while (1) {
    ncols = fscanf(fp2,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoNoise2->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise
  n=0;
  TBranch *catNoise2 = oscillData->Branch("catNoise2", &voltage, "catNoise2/F");
  while (1) {
    ncols = fscanf(fp2,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catNoise2->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal
  n=0;
  TBranch *anoSignal2 = oscillData->Branch("anoSignal2", &voltage, "anoSignal2/F");
  while (1) {
    ncols = fscanf(fp2,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoSignal2->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal
  n=0;
  TBranch *catSignal2 = oscillData->Branch("catSignal2", &voltage, "catSignal2/F");
  while (1) {
    ncols = fscanf(fp2,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catSignal2->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode noise, smoothed
  n=0;
  TBranch *anoNoiseSmooth2 = oscillData->Branch("anoNoiseSmooth2", &voltage, "anoNoiseSmooth2/F");
  while (1) {
    ncols = fscanf(fp2,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoNoiseSmooth2->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise, smoothed
  n=0;
  TBranch *catNoiseSmooth2 = oscillData->Branch("catNoiseSmooth2", &voltage, "catNoiseSmooth2/F");
  while (1) {
    ncols = fscanf(fp2,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catNoiseSmooth2->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal, smoothed
  n=0;
  TBranch *anoSignalSmooth2 = oscillData->Branch("anoSignalSmooth2", &voltage, "anoSignalSmooth2/F");
  while (1) {
    ncols = fscanf(fp2,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoSignalSmooth2->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal, smooth
  n=0;
  TBranch *catSignalSmooth2 = oscillData->Branch("catSignalSmooth2", &voltage, "catSignalSmooth2/F");
  while (1) {
    ncols = fscanf(fp2,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catSignalSmooth2->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }






  // Select file to plot data from
  FILE *fp3 = fopen(TraceFiles[3].Data(),"r");
  // float time, voltage;
  // int month, day, year, hour, minute, second;
  // char AMorPM[2];
  // int n = 0;
  // Definte ncols and skip first line
  ncols = fscanf(fp3,"%d/%d/%d %d:%d:%d %c%c  Pass = 0",&month,&day,&year,&hour,&minute,&second,&AMorPM[0],&AMorPM[1]);
  std::cout << month << "/" << day << "/" << year <<" " << 
    hour << ":" << minute << ":" << second << " " << AMorPM[0] << AMorPM[1] << std::endl;
  pThingString = AMorPM[0];
  if(pThingString == "P" && hour < 12)
    hour+=12;
  TDatime datime3(year,month,day,hour,minute,second);
  datime3.Print();
  // Read in time and anode noise
  n=0;
  TBranch *anoNoise3 = oscillData->Branch("anoNoise3", &voltage, "anoNoise3/F");
  while (1) {
    ncols = fscanf(fp3,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoNoise3->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise
  n=0;
  TBranch *catNoise3 = oscillData->Branch("catNoise3", &voltage, "catNoise3/F");
  while (1) {
    ncols = fscanf(fp3,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catNoise3->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal
  n=0;
  TBranch *anoSignal3 = oscillData->Branch("anoSignal3", &voltage, "anoSignal3/F");
  while (1) {
    ncols = fscanf(fp3,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoSignal3->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal
  n=0;
  TBranch *catSignal3 = oscillData->Branch("catSignal3", &voltage, "catSignal3/F");
  while (1) {
    ncols = fscanf(fp3,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catSignal3->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode noise, smoothed
  n=0;
  TBranch *anoNoiseSmooth3 = oscillData->Branch("anoNoiseSmooth3", &voltage, "anoNoiseSmooth3/F");
  while (1) {
    ncols = fscanf(fp3,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoNoiseSmooth3->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise, smoothed
  n=0;
  TBranch *catNoiseSmooth3 = oscillData->Branch("catNoiseSmooth3", &voltage, "catNoiseSmooth3/F");
  while (1) {
    ncols = fscanf(fp3,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catNoiseSmooth3->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal, smoothed
  n=0;
  TBranch *anoSignalSmooth3 = oscillData->Branch("anoSignalSmooth3", &voltage, "anoSignalSmooth3/F");
  while (1) {
    ncols = fscanf(fp3,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoSignalSmooth3->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal, smooth
  n=0;
  TBranch *catSignalSmooth3 = oscillData->Branch("catSignalSmooth3", &voltage, "catSignalSmooth3/F");
  while (1) {
    ncols = fscanf(fp3,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catSignalSmooth3->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }




  

  // Select file to plot data from
  FILE *fp4 = fopen(TraceFiles[4].Data(),"r");
  // float time, voltage;
  // int month, day, year, hour, minute, second;
  // char AMorPM[2];
  // int n = 0;
  // Definte ncols and skip first line
  ncols = fscanf(fp4,"%d/%d/%d %d:%d:%d %c%c  Pass = 0",&month,&day,&year,&hour,&minute,&second,&AMorPM[0],&AMorPM[1]);
  std::cout << month << "/" << day << "/" << year <<" " << 
    hour << ":" << minute << ":" << second << " " << AMorPM[0] << AMorPM[1] << std::endl;
  pThingString = AMorPM[0];
  if(pThingString == "P" && hour < 12)
    hour+=12;
  TDatime datime4(year,month,day,hour,minute,second);
  datime4.Print();
  // Read in time and anode noise
  n=0;
  TBranch *anoNoise4 = oscillData->Branch("anoNoise4", &voltage, "anoNoise4/F");
  while (1) {
    ncols = fscanf(fp4,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoNoise4->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise
  n=0;
  TBranch *catNoise4 = oscillData->Branch("catNoise4", &voltage, "catNoise4/F");
  while (1) {
    ncols = fscanf(fp4,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catNoise4->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal
  n=0;
  TBranch *anoSignal4 = oscillData->Branch("anoSignal4", &voltage, "anoSignal4/F");
  while (1) {
    ncols = fscanf(fp4,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoSignal4->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal
  n=0;
  TBranch *catSignal4 = oscillData->Branch("catSignal4", &voltage, "catSignal4/F");
  while (1) {
    ncols = fscanf(fp4,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catSignal4->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode noise, smoothed
  n=0;
  TBranch *anoNoiseSmooth4 = oscillData->Branch("anoNoiseSmooth4", &voltage, "anoNoiseSmooth4/F");
  while (1) {
    ncols = fscanf(fp4,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoNoiseSmooth4->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise, smoothed
  n=0;
  TBranch *catNoiseSmooth4 = oscillData->Branch("catNoiseSmooth4", &voltage, "catNoiseSmooth4/F");
  while (1) {
    ncols = fscanf(fp4,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catNoiseSmooth4->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal, smoothed
  n=0;
  TBranch *anoSignalSmooth4 = oscillData->Branch("anoSignalSmooth4", &voltage, "anoSignalSmooth4/F");
  while (1) {
    ncols = fscanf(fp4,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoSignalSmooth4->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal, smooth
  n=0;
  TBranch *catSignalSmooth4 = oscillData->Branch("catSignalSmooth4", &voltage, "catSignalSmooth4/F");
  while (1) {
    ncols = fscanf(fp4,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catSignalSmooth4->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
































  //Find minimum and maximum voltages, needed to get range right for plotting
  int nEntries = (int)oscillData->GetEntries();
  float anoNoiseVolt0, anoSignalVolt0, catNoiseVolt0, catSignalVolt0;
  float anoNoiseVolt1, anoSignalVolt1, catNoiseVolt1, catSignalVolt1;
  float anoNoiseVolt2, anoSignalVolt2, catNoiseVolt2, catSignalVolt2;
  float anoNoiseVolt3, anoSignalVolt3, catNoiseVolt3, catSignalVolt3;
  float anoNoiseVolt4, anoSignalVolt4, catNoiseVolt4, catSignalVolt4;
  float minVoltage=999999, maxVoltage=-999999;
  oscillData->SetBranchAddress("anoNoise0",&anoNoiseVolt0);
  oscillData->SetBranchAddress("anoSignal0",&anoSignalVolt0);
  oscillData->SetBranchAddress("catNoise0",&catNoiseVolt0);
  oscillData->SetBranchAddress("catSignal0",&catSignalVolt0);
  oscillData->SetBranchAddress("anoNoise1",&anoNoiseVolt1);
  oscillData->SetBranchAddress("anoSignal1",&anoSignalVolt1);
  oscillData->SetBranchAddress("catNoise1",&catNoiseVolt1);
  oscillData->SetBranchAddress("catSignal1",&catSignalVolt1);
  oscillData->SetBranchAddress("anoNoise2",&anoNoiseVolt2);
  oscillData->SetBranchAddress("anoSignal2",&anoSignalVolt2);
  oscillData->SetBranchAddress("catNoise2",&catNoiseVolt2);
  oscillData->SetBranchAddress("catSignal2",&catSignalVolt2);
  oscillData->SetBranchAddress("anoNoise3",&anoNoiseVolt3);
  oscillData->SetBranchAddress("anoSignal3",&anoSignalVolt3);
  oscillData->SetBranchAddress("catNoise3",&catNoiseVolt3);
  oscillData->SetBranchAddress("catSignal3",&catSignalVolt3);
  oscillData->SetBranchAddress("anoNoise4",&anoNoiseVolt4);
  oscillData->SetBranchAddress("anoSignal4",&anoSignalVolt4);
  oscillData->SetBranchAddress("catNoise4",&catNoiseVolt4);
  oscillData->SetBranchAddress("catSignal4",&catSignalVolt4);



  // Now we create the averaged (even more), smoothed traces 
  // Begin with signal traces 
  TBranch *anoSignalNotSmooth = oscillData->Branch("anoSignalNotSmooth", &voltage, "anoSignalNotSmooth/F");
  for(int i = 0; i < nEntries; i++){
    oscillData->GetEntry(i);
    voltage = (anoSignalVolt0+anoSignalVolt1+anoSignalVolt2+anoSignalVolt3+anoSignalVolt4)/4;
    // if(i < 5)
      // std::cout << voltage << " " << anoSignalVolt0 << " " << anoSignalVolt1 << " " << anoSignalVolt2 << " " << anoSignalVolt3 << std::endl; 
    anoSignalNotSmooth->Fill();
  }
  TBranch *catSignalNotSmooth = oscillData->Branch("catSignalNotSmooth", &voltage, "catSignalNotSmooth/F");
  for(int i = 0; i < nEntries; i++){
    oscillData->GetEntry(i);
    voltage = (catSignalVolt0+catSignalVolt1+catSignalVolt2+catSignalVolt3+catSignalVolt4)/4;
    catSignalNotSmooth->Fill();
  }

  float anoSignalVoltAverage;
  oscillData->SetBranchAddress("anoSignalNotSmooth",&anoSignalVoltAverage);
  TBranch *anoSignalSmooth = oscillData->Branch("anoSignalSmooth", &voltage, "anoSignalSmooth/F");
  float voltSum = 0;
  for(int i = 0; i <= ISmooth - 1; i++){
    oscillData->GetEntry(i);
    voltSum+= anoSignalVoltAverage;
  }
  for(int i = 0; i <= ISmooth/2 - 1; i++){
    oscillData->GetEntry(i);
    voltage = voltSum/ISmooth;
    anoSignalSmooth->Fill();
  }
  for(int i = ISmooth/2; i <= nEntries - ISmooth/2-1; i++){
    voltSum = 0;
    for(int j = i - ISmooth/2; j <= i + ISmooth/2; j++){
      oscillData->GetEntry(j);
      voltSum+=anoSignalVoltAverage;
    }
    voltage = voltSum/(ISmooth+1);
    anoSignalSmooth->Fill();
  }
  for(int i = nEntries-ISmooth/2; i <= nEntries; i++){
    oscillData->GetEntry(i);
    voltage = voltSum/ISmooth;
    anoSignalSmooth->Fill();
  }
  float catSignalVoltAverage;
  oscillData->SetBranchAddress("catSignalNotSmooth",&catSignalVoltAverage);
  TBranch *catSignalSmooth = oscillData->Branch("catSignalSmooth", &voltage, "catSignalSmooth/F");
  voltSum = 0;
  for(int i = 0; i <= ISmooth - 1; i++){
    oscillData->GetEntry(i);
    voltSum+= catSignalVoltAverage;
  }
  for(int i = 0; i <= ISmooth/2 - 1; i++){
    oscillData->GetEntry(i);
    voltage = voltSum/ISmooth;
    catSignalSmooth->Fill();
  }
  for(int i = ISmooth/2; i <= nEntries - ISmooth/2-1; i++){
    voltSum = 0;
    for(int j = i - ISmooth/2; j <= i + ISmooth/2; j++){
      oscillData->GetEntry(j);
      voltSum+=catSignalVoltAverage;
    }
    voltage = voltSum/(ISmooth+1);
    catSignalSmooth->Fill();
  }
  for(int i = nEntries-ISmooth/2; i <= nEntries; i++){
    oscillData->GetEntry(i);
    voltage = voltSum/ISmooth;
    catSignalSmooth->Fill();
  }

  // Continue with the noise samples 
  TBranch *anoNoiseNotSmooth = oscillData->Branch("anoNoiseNotSmooth", &voltage, "anoNoiseNotSmooth/F");
  for(int i = 0; i < nEntries; i++){
    oscillData->GetEntry(i);
    voltage = (anoNoiseVolt0+anoNoiseVolt1+anoNoiseVolt2+anoNoiseVolt3+anoNoiseVolt4)/4;
    // if(i < 5)
      // std::cout << voltage << " " << anoNoiseVolt0 << " " << anoNoiseVolt1 << " " << anoNoiseVolt2 << " " << anoNoiseVolt3 << std::endl; 
    anoNoiseNotSmooth->Fill();
  }
  TBranch *catNoiseNotSmooth = oscillData->Branch("catNoiseNotSmooth", &voltage, "catNoiseNotSmooth/F");
  for(int i = 0; i < nEntries; i++){
    oscillData->GetEntry(i);
    voltage = (catNoiseVolt0+catNoiseVolt1+catNoiseVolt2+catNoiseVolt3+anoNoiseVolt4)/4;
    catNoiseNotSmooth->Fill();
  }

  float anoNoiseVoltAverage;
  oscillData->SetBranchAddress("anoNoiseNotSmooth",&anoNoiseVoltAverage);
  TBranch *anoNoiseSmooth = oscillData->Branch("anoNoiseSmooth", &voltage, "anoNoiseSmooth/F");
  voltSum = 0;
  for(int i = 0; i <= ISmooth - 1; i++){
    oscillData->GetEntry(i);
    voltSum+= anoNoiseVoltAverage;
  }
  for(int i = 0; i <= ISmooth/2 - 1; i++){
    oscillData->GetEntry(i);
    voltage = voltSum/ISmooth;
    anoNoiseSmooth->Fill();
  }
  for(int i = ISmooth/2; i <= nEntries - ISmooth/2-1; i++){
    voltSum = 0;
    for(int j = i - ISmooth/2; j <= i + ISmooth/2; j++){
      oscillData->GetEntry(j);
      voltSum+=anoNoiseVoltAverage;
    }
    voltage = voltSum/(ISmooth+1);
    anoNoiseSmooth->Fill();
  }
  for(int i = nEntries-ISmooth/2; i <= nEntries; i++){
    oscillData->GetEntry(i);
    voltage = voltSum/ISmooth;
    anoNoiseSmooth->Fill();
  }
  float catNoiseVoltAverage;
  oscillData->SetBranchAddress("catNoiseNotSmooth",&catNoiseVoltAverage);
  TBranch *catNoiseSmooth = oscillData->Branch("catNoiseSmooth", &voltage, "catNoiseSmooth/F");
  voltSum = 0;
  for(int i = 0; i <= ISmooth - 1; i++){
    oscillData->GetEntry(i);
    voltSum+= catNoiseVoltAverage;
  }
  for(int i = 0; i <= ISmooth/2 - 1; i++){
    oscillData->GetEntry(i);
    voltage = voltSum/ISmooth;
    catNoiseSmooth->Fill();
  }
  for(int i = ISmooth/2; i <= nEntries - ISmooth/2-1; i++){
    voltSum = 0;
    for(int j = i - ISmooth/2; j <= i + ISmooth/2; j++){
      oscillData->GetEntry(j);
      voltSum+=catNoiseVoltAverage;
    }
    voltage = voltSum/(ISmooth+1);
    catNoiseSmooth->Fill();
  }
  for(int i = nEntries-ISmooth/2; i <= nEntries; i++){
    oscillData->GetEntry(i);
    voltage = voltSum/ISmooth;
    catNoiseSmooth->Fill();
  }

























  float anoSignalVoltAverageSmooth;
  oscillData->SetBranchAddress("anoSignalSmooth",&anoSignalVoltAverageSmooth);
  float catSignalVoltAverageSmooth;
  oscillData->SetBranchAddress("catSignalSmooth",&catSignalVoltAverageSmooth);
  for(int i = 0; i < nEntries; i++){
    voltage = oscillData->GetEntry(i);
    // if(anoSignalVolt - catSignalVolt < minVoltage)
    //    minVoltage=anoSignalVolt-catSignalVolt;
    // if(anoSignalVolt - catSignalVolt > maxVoltage)
    //    maxVoltage=anoSignalVolt-catSignalVolt;

    // if(anoSignalVolt - catSignalVolt < minVoltage)
    //    minVoltage=anoSignalVolt-catSignalVolt;
    // if(anoSignalVolt - catSignalVolt > maxVoltage)
    //    maxVoltage=anoSignalVolt-catSignalVolt;


    if(anoSignalVoltAverageSmooth< minVoltage)
      minVoltage=anoSignalVoltAverageSmooth;
    if(catSignalVoltAverageSmooth< minVoltage)
      minVoltage=catSignalVoltAverageSmooth;
    if(anoSignalVoltAverageSmooth> maxVoltage)
      maxVoltage=anoSignalVoltAverageSmooth;
    if(catSignalVoltAverageSmooth> maxVoltage)
      maxVoltage=catSignalVoltAverageSmooth;


    // if(anoSignalVolt-anoNoiseVolt < minVoltage)
    //   minVoltage=anoSignalVolt-anoNoiseVolt;
    // if(catSignalVolt-catNoiseVolt < minVoltage)
    //   minVoltage=catSignalVolt-catNoiseVolt;
    // if(anoSignalVolt-anoNoiseVolt > maxVoltage)
    //   maxVoltage=anoSignalVolt-anoNoiseVolt;
    // if(catSignalVolt-catNoiseVolt > maxVoltage)
    //   maxVoltage=catSignalVolt-catNoiseVolt;
    // if(anoSignalVolt+0.020 < minVoltage)
    //   minVoltage=anoSignalVolt+0.020;
    // if(catSignalVolt+0.020 < minVoltage)
    //   minVoltage=catSignalVolt+0.020;
    // if(anoSignalVolt+0.020 > maxVoltage)
    //   maxVoltage=anoSignalVolt+0.020;
    // if(catSignalVolt+0.020 > maxVoltage)
    //   maxVoltage=catSignalVolt+0.020;
    // if(anoNoiseVolt+0.040 > maxVoltage)
    //   maxVoltage=anoNoiseVolt+0.040;
    // if(catNoiseVolt+0.040 > maxVoltage)
    //   maxVoltage=catNoiseVolt+0.040;

  }


  /*Plot the oscilloscope scans*/
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
  oscillData->Draw("1000*(catSignalSmooth-catNoiseSmooth):1000*time","","LSAME");
  TGraph *graph = (TGraph*)gPad->GetPrimitive("Graph");
  frame->GetXaxis()->SetTitle("ms");
  frame->GetYaxis()->SetTitle("mV");
  leg->AddEntry(graph,"cathode signal","l");
  oscillData->SetMarkerStyle(5);
  oscillData->SetMarkerColor(4);
  oscillData->SetLineColor(4);
  oscillData->SetLineStyle(1);
  oscillData->SetLineWidth(4);
  oscillData->Draw("1000*(anoSignalSmooth-anoNoiseSmooth):1000*time","","LSAME");
  TGraph *graph1 = (TGraph*)gPad->GetPrimitive("Graph")->Clone();
  graph1->SetMarkerStyle(5);
  graph1->SetMarkerColor(4);
  graph1->SetLineColor(4);
  graph1->SetLineWidth(4);
  leg->AddEntry(graph1,"anode signal","l");



  // oscillData->SetMarkerStyle(2);
  // oscillData->SetMarkerColor(8);
  // oscillData->SetLineColor(8);
  // oscillData->SetLineStyle(3);
  // oscillData->Draw("1000*(catSignalSmooth)+20:1000*time","","LSAME");
  // TGraph *graph2 = (TGraph*)gPad->GetPrimitive("Graph")->Clone();
  // graph2->SetMarkerStyle(2);
  // graph2->SetMarkerColor(8);
  // graph2->SetLineColor(8);
  // leg->AddEntry(graph2,"cathode signal, without noise subtracted","l");

  // oscillData->SetMarkerStyle(5);
  // oscillData->SetLineStyle(4);
  // oscillData->SetMarkerColor(1);
  // oscillData->SetLineColor(1);
  // oscillData->Draw("1000*(anoSignalSmooth)+20:1000*time","","LSAME");
  // TGraph *graph3 = (TGraph*)gPad->GetPrimitive("Graph")->Clone();
  // graph3->SetMarkerStyle(5);
  // graph3->SetMarkerColor(1);
  // graph3->SetLineColor(1);
  // leg->AddEntry(graph3,"anode signal, without noise subtracted","l");


  // oscillData->SetMarkerStyle(2);
  // oscillData->SetMarkerColor(28);
  // oscillData->SetLineColor(28);
  // oscillData->Draw("1000*(catNoiseSmooth)+40:1000*time","","LSAME");
  // TGraph *graph4 = (TGraph*)gPad->GetPrimitive("Graph")->Clone();
  // graph4->SetMarkerStyle(2);
  // graph4->SetMarkerColor(28);
  // graph4->SetLineColor(28);
  // leg->AddEntry(graph4,"cathode noise","l");

  // oscillData->SetMarkerStyle(5);
  // oscillData->SetMarkerColor(6);
  // oscillData->SetLineColor(6);
  // oscillData->Draw("1000*(anoNoiseSmooth)+40:1000*time","","LSAME");
  // TGraph *graph5 = (TGraph*)gPad->GetPrimitive("Graph")->Clone();
  // graph5->SetMarkerStyle(5);
  // graph5->SetMarkerColor(6);
  // graph5->SetLineColor(6);
  // leg->AddEntry(graph5,"anode noise","l");

  leg->Draw();

  TString PrMTrace = TraceFiles[0].ReplaceAll(".txt",".png");
  c1->Print(PrMTrace);











  calc.CalculateLifetime(oscillData, atoi(sPrM.Data()), doNoiseSubtraction);
  // Print lifetime out to a text file 
  TString outFileName = "PrM_Logs/lifetimes_0"+sPrM+".txt";
  ofstream myfile;
  myfile.open(outFileName.Data(),ios::app);
  myfile << runNumber.Data() << " " 
    << datime.GetMonth() << " " 
    << datime.GetDay() << " " 
    << datime.GetYear() << " " 
    << datime.GetHour() << " " 
    << datime.GetMinute() << " " 
    << datime.GetSecond() << " " 
    << calc.Lifetime() << " " 
    << calc.CatTrue() << " " 
    << calc.AnoTrue() << " " 
    << calc.CatBase() << " " 
    << calc.AnoBase() << " " 
    << calc.CatRMS() << " " 
    << calc.AnoRMS() << " " 
    << calc.CathF() << " " 
    << calc.AnoF() << 
    "\n";
  myfile.close();





  TString when = Form("%d",datime.GetMonth());
  when+="/";
  when+=Form("%d",datime.GetDay());
  when+="/";
  when+=Form("%d",datime.GetYear());
  when+=" ";
  when+=Form("%02d",datime.GetHour());
  when+=":";
  when+=Form("%02d",datime.GetMinute());
  when+=":";
  when+=Form("%02d",datime.GetSecond());
  std::cout << when.Data() << std::endl;


  // TString outCSVFileName = "Run"+runNumber+"."+sPrM+".LogData.csv";
  // myfile.open(outCSVFileName.Data());
  // myfile << "[Data]\n";
  // myfile << "Tagname,TimeStamp,Value\n";
  // myfile << "UBOONE.PRM_CATHPEAK_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.CatPeak() << "\n";
  // myfile << "UBOONE.PRM_CATHTIME_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.CatTime() << "\n";
  // myfile << "UBOONE.PRM_CATHBASE_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.CatBase() << "\n";
  // myfile << "UBOONE.PRM_ANODEPEAK_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoPeak() << "\n";
  // myfile << "UBOONE.PRM_ANODETIME_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoTime() << "\n";
  // myfile << "UBOONE.PRM_ANODEBASE_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoBase() << "\n";
  // myfile << "UBOONE.PRM_ANODERISE_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoRise() << "\n";
  // myfile << "UBOONE.PRM_CATHFACTOR_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.CathF() << "\n";
  // myfile << "UBOONE.PRM_ANODEFACTOR_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoF() << "\n";
  // myfile << "UBOONE.PRM_ANODETRUE_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoTrue() << "\n";
  // myfile << "UBOONE.PRM_CATHTRUE_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.CatTrue() << "\n";
  // myfile << "UBOONE.PRM_LIFETIME_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.Lifetime() << "\n";
  // myfile << "UBOONE.PRM_IMPURITIES_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.Impurities() << "\n";
  // myfile.close();




}
