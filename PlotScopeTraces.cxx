
#include "PlotScopeTraces.h"
#include "TString.h"

// Plot the data coming from the digitizer card reading the purity monitors



void PlotScopeTraces::RunPlotAndLifetime(TString PrMFile){
  TNtuple *oscillData = new TNtuple("oscillData","NTUPLE","time:anoNoise");

  TString sPrM = PrMFile[23];
  TString runNumber = PrMFile(15,6);

  // Subtract noise traces from signal?
  bool doNoiseSubtraction = false;
  if(atoi(runNumber.Data()) >= 1816 && atoi(sPrM.Data()) == 1)
    doNoiseSubtraction = true;
  if(atoi(sPrM.Data()) == 0)
    doNoiseSubtraction = true;


  // Select file to plot data from
  FILE *fp = fopen(PrMFile.Data(),"r");
  float time, voltage;
  int month, day, year, hour, minute, second;
  char AMorPM[2];
  int n = 0;
  // Definte ncols and skip first line
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
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    oscillData->Fill(time,voltage);
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise
  n=0;
  TBranch *catNoise = oscillData->Branch("catNoise", &voltage, "catNoise/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catNoise->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal
  n=0;
  TBranch *anoSignal = oscillData->Branch("anoSignal", &voltage, "anoSignal/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoSignal->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal
  n=0;
  TBranch *catSignal = oscillData->Branch("catSignal", &voltage, "catSignal/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catSignal->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode noise, smoothed
  n=0;
  TBranch *anoNoiseSmooth = oscillData->Branch("anoNoiseSmooth", &voltage, "anoNoiseSmooth/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoNoiseSmooth->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode noise, smoothed
  n=0;
  TBranch *catNoiseSmooth = oscillData->Branch("catNoiseSmooth", &voltage, "catNoiseSmooth/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catNoiseSmooth->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in anode signal, smoothed
  n=0;
  TBranch *anoSignalSmooth = oscillData->Branch("anoSignalSmooth", &voltage, "anoSignalSmooth/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    anoSignalSmooth->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode signal, smooth
  n=0;
  TBranch *catSignalSmooth = oscillData->Branch("catSignalSmooth", &voltage, "catSignalSmooth/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    // if (n < 5)
    //   std::cout << time << " " << voltage << std::endl;
    catSignalSmooth->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }





  //Find minimum and maximum voltages, needed to get range right for plotting
  int nEntries = (int)oscillData->GetEntries();
  float anoNoiseVolt, anoSignalVolt, catNoiseVolt, catSignalVolt;
  float minVoltage=999999, maxVoltage=-999999;
  oscillData->SetBranchAddress("anoNoiseSmooth",&anoNoiseVolt);
  oscillData->SetBranchAddress("anoSignalSmooth",&anoSignalVolt);
  oscillData->SetBranchAddress("catNoiseSmooth",&catNoiseVolt);
  oscillData->SetBranchAddress("catSignalSmooth",&catSignalVolt);
  for(int i = 0; i < nEntries; i++){
    voltage = oscillData->GetEntry(i);

    if(anoSignalVolt < minVoltage)
      minVoltage=anoSignalVolt;
    if(catSignalVolt < minVoltage)
      minVoltage=catSignalVolt;
    if(anoSignalVolt > maxVoltage)
      maxVoltage=anoSignalVolt;
    if(catSignalVolt > maxVoltage)
      maxVoltage=catSignalVolt;

    // if(anoNoiseVolt + 0.010 < minVoltage)
    //   minVoltage=anoNoiseVolt + 0.010;
    // if(catNoiseVolt + 0.010 < minVoltage)
    //   minVoltage=catNoiseVolt + 0.010;
    // if(anoNoiseVolt + 0.010 > maxVoltage)
    //   maxVoltage=anoNoiseVolt + 0.010;
    // if(catNoiseVolt + 0.010 > maxVoltage)
    //   maxVoltage=catNoiseVolt + 0.010;


  }

  std::cout << maxVoltage << " " << minVoltage << std::endl;

  /*Plot the oscilloscope scans*/
  TCanvas *c1 = new TCanvas("c1","Tree test");
  TH2F *frame = new TH2F("frame","", 100, 1000*(oscillData->GetMinimum("time")), 1000*(oscillData->GetMaximum("time")), 100, 1000*(minVoltage-0.1*fabs(maxVoltage-minVoltage)), 1000*(maxVoltage+0.5*fabs(maxVoltage-minVoltage)));
  gStyle->SetOptStat(0);
  gPad->SetTicks(0,1);
  frame->Draw();
  TLegend *leg = new TLegend(0.12,0.68,0.59,0.88);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  oscillData->SetMarkerStyle(2);
  oscillData->SetMarkerColor(kOrange+8);
  oscillData->SetLineColor(kOrange+8);
  oscillData->SetLineWidth(4);
  oscillData->SetLineStyle(2);
  oscillData->SetMarkerSize(0.5);
  if(doNoiseSubtraction)
    oscillData->Draw("1000*(catSignalSmooth-catNoiseSmooth):1000*time","","LSAME");
  else 
    oscillData->Draw("1000*(catSignalSmooth):1000*time","","LSAME");
  TGraph *graph = (TGraph*)gPad->GetPrimitive("Graph");
  frame->GetXaxis()->SetTitle("ms");
  frame->GetYaxis()->SetTitle("mV");
  leg->AddEntry(graph,"cathode signal","l");
  oscillData->SetMarkerStyle(5);
  oscillData->SetMarkerColor(4);
  oscillData->SetLineColor(4);
  oscillData->SetLineStyle(1);
  oscillData->SetLineWidth(4);
  if(doNoiseSubtraction)
    oscillData->Draw("1000*(anoSignalSmooth-anoNoiseSmooth):1000*time","","LSAME");
  else
    oscillData->Draw("1000*(anoSignalSmooth):1000*time","","LSAME");
  TGraph *graph1 = (TGraph*)gPad->GetPrimitive("Graph")->Clone();
  graph1->SetMarkerStyle(5);
  graph1->SetMarkerColor(4);
  graph1->SetLineColor(4);
  graph1->SetLineWidth(4);
  leg->AddEntry(graph1,"anode signal","l");









//   oscillData->SetMarkerStyle(2);
//   oscillData->SetMarkerColor(8);
//   oscillData->SetLineColor(8);
//   oscillData->SetLineStyle(3);
//   oscillData->Draw("1000*(catSignalSmooth)+10:1000*time","","LSAME");
//   TGraph *graph2 = (TGraph*)gPad->GetPrimitive("Graph")->Clone();
//   graph2->SetMarkerStyle(2);
//   graph2->SetMarkerColor(8);
//   graph2->SetLineColor(8);
//   leg->AddEntry(graph2,"cathode signal, without noise subtracted","l");

//   oscillData->SetMarkerStyle(5);
//   oscillData->SetLineStyle(4);
//   oscillData->SetMarkerColor(1);
//   oscillData->SetLineColor(1);
//   oscillData->Draw("1000*(anoSignalSmooth)+10:1000*time","","LSAME");
//   TGraph *graph3 = (TGraph*)gPad->GetPrimitive("Graph")->Clone();
//   graph3->SetMarkerStyle(5);
//   graph3->SetMarkerColor(1);
//   graph3->SetLineColor(1);
//   leg->AddEntry(graph3,"anode signal, without noise subtracted","l");







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

  TString PrMTrace = PrMFile.ReplaceAll(".txt",".png");
  c1->Print(PrMTrace);

  TString PrMTracePS = PrMFile.ReplaceAll(".png",".ps");
  c1->Print(PrMTracePS);

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
