
#include "PlotScopeTracesAverage.h"
#include "TString.h"

// Plot the data coming from the digitizer card reading the purity monitors



void PlotScopeTracesAverage::RunPlotAndLifetime(TString PrMFile0, TString PrMFile1, TString PrMFile2, TString PrMFile3){
  TNtuple *oscillData = new TNtuple("oscillData","NTUPLE","time:anoNoise0");

  // Select file to plot data from
  FILE *fp = fopen(PrMFile0.Data(),"r");
  float time, voltage;
  int month, day, year, hour, minute, second, pass;
  char AMorPM[2];
  int n = 0;
  // Definte ncols and skip first line
  int ncols = fscanf(fp,"%d/%d/%d %d:%d:%d %c%c Pass = %d",&month,&day,&year,&hour,&minute,&second,&AMorPM[0],&AMorPM[1],&pass);
  std::cout << month << "/" << day << "/" << year <<" " << 
    hour << ":" << minute << ":" << second << " " << AMorPM << 
    " Pass = " << pass << std::endl;
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
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
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
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
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
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
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
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
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
  FILE *fp1 = fopen(PrMFile1.Data(),"r");
  // float time, voltage;
  // int month, day, year, hour, minute, second, pass;
  // char AMorPM[2];
  // int n = 0;
  // Definte ncols and skip first line
  ncols = fscanf(fp1,"%d/%d/%d %d:%d:%d %c%c Pass = %d",&month,&day,&year,&hour,&minute,&second,&AMorPM[0],&AMorPM[1],&pass);
  std::cout << month << "/" << day << "/" << year <<" " << 
    hour << ":" << minute << ":" << second << " " << AMorPM << 
    " Pass = " << pass << std::endl;
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
  FILE *fp2 = fopen(PrMFile2.Data(),"r");
  // float time, voltage;
  // int month, day, year, hour, minute, second, pass;
  // char AMorPM[2];
  // int n = 0;
  // Definte ncols and skip first line
  ncols = fscanf(fp2,"%d/%d/%d %d:%d:%d %c%c Pass = %d",&month,&day,&year,&hour,&minute,&second,&AMorPM[0],&AMorPM[1],&pass);
  std::cout << month << "/" << day << "/" << year <<" " << 
    hour << ":" << minute << ":" << second << " " << AMorPM << 
    " Pass = " << pass << std::endl;
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
  FILE *fp3 = fopen(PrMFile3.Data(),"r");
  // float time, voltage;
  // int month, day, year, hour, minute, second, pass;
  // char AMorPM[2];
  // int n = 0;
  // Definte ncols and skip first line
  ncols = fscanf(fp3,"%d/%d/%d %d:%d:%d %c%c Pass = %d",&month,&day,&year,&hour,&minute,&second,&AMorPM[0],&AMorPM[1],&pass);
  std::cout << month << "/" << day << "/" << year <<" " << 
    hour << ":" << minute << ":" << second << " " << AMorPM << 
    " Pass = " << pass << std::endl;
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



































  //Find minimum and maximum voltages, needed to get range right for plotting
  int nEntries = (int)oscillData->GetEntries();
  float anoNoiseVolt0, anoSignalVolt0, catNoiseVolt0, catSignalVolt0;
  float anoNoiseVolt1, anoSignalVolt1, catNoiseVolt1, catSignalVolt1;
  float anoNoiseVolt2, anoSignalVolt2, catNoiseVolt2, catSignalVolt2;
  float anoNoiseVolt3, anoSignalVolt3, catNoiseVolt3, catSignalVolt3;
  float minVoltage=999999, maxVoltage=-999999;
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


    if(anoSignalVolt1 < minVoltage)
      minVoltage=anoSignalVolt1;
    if(catSignalVolt1 < minVoltage)
      minVoltage=catSignalVolt1;
    if(anoSignalVolt1 > maxVoltage)
      maxVoltage=anoSignalVolt1;
    if(catSignalVolt1 > maxVoltage)
      maxVoltage=catSignalVolt1;


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
  oscillData->Draw("1000*(catSignalSmooth0+catSignalSmooth1+catSignalSmooth2+catSignalSmooth3)/4:1000*time","","LSAME");
  TGraph *graph = (TGraph*)gPad->GetPrimitive("Graph");
  frame->GetXaxis()->SetTitle("ms");
  frame->GetYaxis()->SetTitle("mV");
  leg->AddEntry(graph,"cathode signal","l");
  oscillData->SetMarkerStyle(5);
  oscillData->SetMarkerColor(4);
  oscillData->SetLineColor(4);
  oscillData->SetLineStyle(1);
  oscillData->SetLineWidth(4);
  oscillData->Draw("1000*(anoSignalSmooth0+anoSignalSmooth1+anoSignalSmooth2+anoSignalSmooth3)/4:1000*time","","LSAME");
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

  TString PrMTrace = PrMFile0.ReplaceAll(".txt",".png");
  c1->Print(PrMTrace);









  TString sPrM = PrMFile0[12];
  calc.CalculateLifetime(oscillData, atoi(sPrM.Data()));

  // Print lifetime out to a text file 
  TString runNumber = PrMFile0(4,6);
  TString outFileName = "lifetimes_0"+sPrM+".txt";
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
    << calc.CatRMS() << " " 
    << calc.AnoRMS() << 
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


  TString outCSVFileName = "Run"+runNumber+"."+sPrM+".LogData.csv";
  myfile.open(outCSVFileName.Data());
  myfile << "[Data]\n";
  myfile << "Tagname,TimeStamp,Value\n";
  myfile << "UBOONE.PRM_CATHPEAK_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.CatPeak() << "\n";
  myfile << "UBOONE.PRM_CATHTIME_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.CatTime() << "\n";
  myfile << "UBOONE.PRM_CATHBASE_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.CatBase() << "\n";
  myfile << "UBOONE.PRM_ANODEPEAK_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoPeak() << "\n";
  myfile << "UBOONE.PRM_ANODETIME_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoTime() << "\n";
  myfile << "UBOONE.PRM_ANODEBASE_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoBase() << "\n";
  myfile << "UBOONE.PRM_ANODERISE_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoRise() << "\n";
  myfile << "UBOONE.PRM_CATHFACTOR_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.CathF() << "\n";
  myfile << "UBOONE.PRM_ANODEFACTOR_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoF() << "\n";
  myfile << "UBOONE.PRM_ANODETRUE_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.AnoTrue() << "\n";
  myfile << "UBOONE.PRM_CATHTRUE_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.CatTrue() << "\n";
  myfile << "UBOONE.PRM_LIFETIME_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.Lifetime() << "\n";
  myfile << "UBOONE.PRM_IMPURITIES_0" << atoi(sPrM.Data()) << ".F_CV," << when.Data() << "," << calc.Impurities() << "\n";
  myfile.close();




}
