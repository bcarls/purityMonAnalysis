

#include "PurityPlotMaker.h"



void PurityPlotMaker::MakePlots(){
  TNtuple *lifetimeData = new TNtuple("lifetimeData","NTUPLE","run:month:day:year:hour:minute:second:lifetime:QC:QA:CathF:AnoF");
  TNtuple *averagedLifetimeData = new TNtuple("averagedLifetime","NTUPLE","averagedRun:averagedLifetime:datime");

  for(int i = 0; i < listOfDataFiles.size(); i++)
    lifetimeData->ReadFile(listOfDataFiles[i].Data());

  // lifetimeData->Scan();

  float runNumber, Lifetime, QC, QA, CathF, AnoF;
  float runNumberSum, LifetimeSum;
  float month, day, year, hour, minute, second;
  int nEntries = (int)lifetimeData->GetEntries();
  lifetimeData->SetBranchAddress("run",&runNumber);
  lifetimeData->SetBranchAddress("month",&month);
  lifetimeData->SetBranchAddress("day",&day);
  lifetimeData->SetBranchAddress("year",&year);
  lifetimeData->SetBranchAddress("hour",&hour);
  lifetimeData->SetBranchAddress("minute",&minute);
  lifetimeData->SetBranchAddress("second",&second);
  lifetimeData->SetBranchAddress("lifetime",&Lifetime);
  lifetimeData->SetBranchAddress("QC",&QC);
  lifetimeData->SetBranchAddress("QA",&QA);
  lifetimeData->SetBranchAddress("CathF",&CathF);
  lifetimeData->SetBranchAddress("AnoF",&AnoF);


  TDatime datime, datimeMin, datimeMax;
  UInt_t datimeMinConvert = 1739935037 ;
  int datimeMaxConvert = 0;
  TBranch *datimeBranch = lifetimeData->Branch("datime", &datime);
  float lifetimeValue;
  float lifetimeMax = 0;
  lifetimeData->SetBranchAddress("lifetime",&lifetimeValue);
  for(int i = 0; i < lifetimeData->GetEntries(); i++){
    lifetimeData->GetEntry(i);

    datime.Set(year,month,day,hour,minute,second);
    datimeBranch->Fill();
    if(lifetimeValue > lifetimeMax && lifetimeValue < 0.1)
      lifetimeMax = lifetimeValue;

    if(datime.Convert() > datimeMaxConvert){
      datimeMaxConvert = datime.Convert();
      datimeMax.Set(year,month,day,hour,minute,second);
    }
    if(datime.Convert() < datimeMinConvert){
      datimeMinConvert = datime.Convert();
      datimeMin.Set(year,month,day,hour,minute,second);
    }

  }

  datimeMin.Print();
  datimeMax.Print();


  TString sAtM = listOfDataFiles[0][20];



  // Do the averaging
  int status;
  int j=0;
  int measPerAve = 6;
  LifetimeSum = 0;
  float datimeSum = 0;
  for(int i = 0; i < nEntries; i++){
    status = lifetimeData->GetEntry(i);
    if(lifetimeValue >= 0.1)
      continue;
    runNumberSum+=runNumber;
    LifetimeSum+=lifetimeValue;
    datimeSum+=datime.Convert();
    j++;
    if(j==measPerAve){
      averagedLifetimeData->Fill(runNumberSum/measPerAve,LifetimeSum/measPerAve,datimeSum/measPerAve);
      runNumberSum=0;
      LifetimeSum=0;
      datimeSum=0;
      j=0;
    }
  }
  if(j!=0)
    averagedLifetimeData->Fill(runNumberSum/(double)j,LifetimeSum/(double)j,datimeSum/(double)j);
  float averagedLifetimeValue;
  float lifetimeStandardDevValue = 0;
  TBranch *lifetimeStandardDev = averagedLifetimeData->Branch("lifetimeStandardDev", &lifetimeStandardDevValue, "lifetimeStandardDev/F");
  averagedLifetimeData->SetBranchAddress("averagedLifetime",&averagedLifetimeValue);
  j = 0;
  for(int i = 0; i < nEntries; i++){
    status = lifetimeData->GetEntry(i);
    status = averagedLifetimeData->GetEntry(i/measPerAve);
    if(lifetimeValue >= 0.1)
      continue;
    lifetimeStandardDevValue += pow(lifetimeValue-averagedLifetimeValue,2);
    j++;
    if(j==measPerAve){
      lifetimeStandardDevValue = sqrt((1/(double)measPerAve)*lifetimeStandardDevValue);
      lifetimeStandardDev->Fill();
      lifetimeStandardDevValue=0; 
      j=0;
    }
  }
  if(j!=0){
    lifetimeStandardDevValue = sqrt((1/(double)j)*lifetimeStandardDevValue);
    lifetimeStandardDev->Fill();
  }















  lifetimeData->SetMarkerStyle(2);
  lifetimeData->SetMarkerColor(2);
  lifetimeData->SetLineColor(2);
  lifetimeData->SetMarkerSize(3.0);

  TCanvas *c1 = new TCanvas("c1","Tree test");
  // Turn grid on
  c1->SetGridx(1);
  c1->SetGridy(1);
  // Turn grid off
  c1->SetGridx(0);
  c1->SetGridy(0);
  gStyle->SetOptStat(0);



  // TH2F *frameLifetime = new TH2F("frameLifetime","", 1000, datimeMin.Convert()-100000, datimeMax.Convert()+100000, 1000, 0, 1.1*1000*lifetimeMax);
  TH2F *frameLifetime = new TH2F("frameLifetime","", 1000, datimeMax.Convert()-604800, datimeMax.Convert(), 1000, 0, 1.1*1000*lifetimeMax);
  frameLifetime->GetXaxis()->SetTitle("date/time");
  frameLifetime->GetXaxis()->SetTimeDisplay(1);
  frameLifetime->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameLifetime->GetXaxis()->SetTimeOffset(0);
  frameLifetime->GetXaxis()->SetLabelOffset(0.025);
  frameLifetime->GetXaxis()->SetTitleOffset(1.5);
  frameLifetime->GetYaxis()->SetTitle("lifetime (ms)");
  // frameLifetime->SetNdivisions(-404);
  frameLifetime->SetNdivisions(-7);
  frameLifetime->Draw();

  lifetimeData->Draw("1000*lifetime:datime.Convert()","lifetime<0.1","SAME");

  TString lifetimeImage = "lifetime_" + sAtM + ".ps";
  TString lifetimeImagePNG = "lifetime_" + sAtM + ".png";

  c1->Print(lifetimeImage);
  std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);

 




  // TH2F *frameQAQC = new TH2F("frameQAQC","", 1000, datimeMin.Convert()-50000, datimeMax.Convert()+50000, 1000, 0, 1);
  TH2F *frameQAQC = new TH2F("frameQAQC","", 1000, datimeMax.Convert()-604800, datimeMax.Convert(), 1000, 0, 1);
  frameQAQC->GetXaxis()->SetTitle("date/time");
  frameQAQC->GetXaxis()->SetTimeDisplay(1);
  frameQAQC->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameQAQC->GetXaxis()->SetTimeOffset(0);
  frameQAQC->GetXaxis()->SetLabelOffset(0.025);
  frameQAQC->GetXaxis()->SetTitleOffset(1.5);
  frameQAQC->GetYaxis()->SetTitle("Q_{A}/Q_{C}");
  frameQAQC->SetNdivisions(-7);
  frameQAQC->Draw();
  lifetimeData->Draw("QA/QC:datime.Convert()","","SAME");
  // TF1 *f3msLifetime = new TF1("f3msLifetime","[0]",datimeMin.Convert()-50000, datimeMax.Convert()+50000);
  TF1 *f3msLifetime = new TF1("f3msLifetime","[0]",datimeMax.Convert()-604800, datimeMax.Convert());
  f3msLifetime->SetParameter(0,exp(-2.82/3)); 
  f3msLifetime->SetLineColor(1);
  f3msLifetime->SetLineStyle(2);
  f3msLifetime->Draw("SAME");
  // TPaveText *lab3ms = new TPaveText(1.43894e9,0.393,1.43903e9,0.443);
  TPaveText *lab3ms = new TPaveText(datimeMax.Convert()-604800+0.00001e9,0.393,datimeMax.Convert()-604800+0.0001e9,0.443);
  lab3ms->SetFillColor(0);
  // lab6ms->SetLineColor(0);
  lab3ms->AddText("3 ms");
  lab3ms->Draw("SAME");
  // TF1 *f9msLifetime = new TF1("f9msLifetime","[0]",datimeMin.Convert()-50000, datimeMax.Convert()+50000);
  TF1 *f9msLifetime = new TF1("f9msLifetime","[0]",datimeMax.Convert()-604800, datimeMax.Convert());
  f9msLifetime->SetParameter(0,exp(-2.82/9)); 
  f9msLifetime->SetLineColor(1);
  f9msLifetime->SetLineStyle(2);
  f9msLifetime->Draw("SAME");
  // TPaveText *lab9ms = new TPaveText(1.43894e9,0.743,1.43903e9,0.793);
  TPaveText *lab9ms = new TPaveText(datimeMax.Convert()-604800+0.00001e9,0.743,datimeMax.Convert()-604800+0.0001e9,0.793);
  lab9ms->SetFillColor(0);
  // lab6ms->SetLineColor(0);
  lab9ms->AddText("9 ms");
  lab9ms->Draw("SAME");

  TString QAQCImage = "QAQC_" + sAtM + ".ps";
  TString QAQCImagePNG = "QAQC_" + sAtM + ".png";

  c1->Print(QAQCImage);
  std::cout << "convert -rotate 90 " + QAQCImage + " "+ QAQCImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + QAQCImage + " "+ QAQCImagePNG );





  // Make the averaged plot
  gStyle->SetOptStat(0);
  TH2F *frameAverageLifetime = new TH2F("frame","", 1000, averagedLifetimeData->GetMinimum("averagedRun")-10, averagedLifetimeData->GetMaximum("averagedRun")+10, 1000, 0, 1.1*1000*averagedLifetimeData->GetMaximum("averagedLifetime"));
  frameAverageLifetime ->GetXaxis()->SetTitle("date/time");
  frameAverageLifetime ->GetXaxis()->SetTimeDisplay(1);
  frameAverageLifetime->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameAverageLifetime->GetXaxis()->SetTimeOffset(0);
  frameAverageLifetime->GetXaxis()->SetLabelOffset(0.025);
  frameAverageLifetime->GetXaxis()->SetTitleOffset(1.5);
  frameAverageLifetime ->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  frameAverageLifetime ->GetXaxis()->SetTimeOffset(0);
  frameAverageLifetime ->Draw();
  long N = averagedLifetimeData->Draw("datime.Convert():1000*averagedLifetime:1000*lifetimeStandardDev","","goff");
  TGraphErrors *gr = new TGraphErrors(N,averagedLifetimeData->GetV1(),averagedLifetimeData->GetV2(),0,averagedLifetimeData->GetV3());
  gr->SetMarkerStyle(2);
  gr->SetMarkerColor(2);
  gr->SetLineColor(2);
  gr->SetMarkerSize(3.0);
  gr->SetMinimum(0);
  gr->GetXaxis()->SetTitle("averaged run number");
  gr->GetYaxis()->SetTitle("averaged lifetime (ms)");
  gr->GetXaxis()->SetTitle("date/time");
  gr->GetXaxis()->SetTimeDisplay(1);
  gr->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  gr->GetXaxis()->SetTimeOffset(0);
  gr->GetXaxis()->SetNdivisions(-7);
  gr->Draw("APESAME");

  TString averageLifetimeImage = "averageLifetime_" + sAtM + ".ps";
  TString averageLifetimeImagePNG = "averageLifetime_" + sAtM + ".png";

  c1->Print(averageLifetimeImage);
  std::cout << "convert -rotate 90 "+averageLifetimeImage+" "+averageLifetimeImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 "+averageLifetimeImage+" "+averageLifetimeImagePNG);






















  // TH2F *frame = new TH2F("frame","", 1000, datimeMin.Convert()-50000, datimeMax.Convert()+50000, 1000, 0, 1);
//   frame->GetXaxis()->SetTitle("date/time");
//   frame->GetXaxis()->SetTimeDisplay(1);
//   frame->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
//   frame->GetXaxis()->SetTimeOffset(0);
//   frame->GetYaxis()->SetTitle("Q_{C}");
//   frame->SetNdivisions(-404);
//   frame->Draw();

//   lifetimeData->Draw("QC:datime.Convert()","","SAME");

//   TString sAtM = lifetimeFile[13];

//   TString lifetimeImage = lifetimeFile.ReplaceAll(".txt",".ps");
//   TString lifetimeImagePNG = lifetimeFile.ReplaceAll(".ps",".png");

//   c1->Print(lifetimeImage);
//   std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
//   gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);




//   TH2F *frame = new TH2F("frame","", 1000, datimeMin.Convert()-2000, datimeMax.Convert()+2000, 1000, 0, 1);
//   frame->GetXaxis()->SetTitle("date/time");
//   frame->GetXaxis()->SetTimeDisplay(1);
//   frame->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
//   frame->GetXaxis()->SetTimeOffset(0);
//   frame->GetYaxis()->SetTitle("Q_{A}");
//   frame->SetNdivisions(-404);
//   frame->Draw();

//   lifetimeData->Draw("QA:datime.Convert()","","SAME");

//   TString sAtM = lifetimeFile[13];

//   TString lifetimeImage = lifetimeFile.ReplaceAll(".txt",".ps");
//   TString lifetimeImagePNG = lifetimeFile.ReplaceAll(".ps",".png");

//   c1->Print(lifetimeImage);
//   std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
//   gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);




//   TH2F *frame = new TH2F("frame","", 1000, datimeMin.Convert()-2000, datimeMax.Convert()+2000, 1000, 0, 2);
//   frame->GetXaxis()->SetTitle("date/time");
//   frame->GetXaxis()->SetTimeDisplay(1);
//   frame->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
//   frame->GetXaxis()->SetTimeOffset(0);
//   frame->GetYaxis()->SetTitle("cathode factor");
//   frame->SetNdivisions(-404);
//   frame->Draw();

//   lifetimeData->Draw("CathF:datime.Convert()","","SAME");

//   TString sAtM = lifetimeFile[13];

//   TString lifetimeImage = lifetimeFile.ReplaceAll(".txt",".ps");
//   TString lifetimeImagePNG = lifetimeFile.ReplaceAll(".ps",".png");

//   c1->Print(lifetimeImage);
//   std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
//   gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);




//   TH2F *frame = new TH2F("frame","", 1000, datimeMin.Convert()-2000, datimeMax.Convert()+2000, 1000, 0, 2);
//   frame->GetXaxis()->SetTitle("date/time");
//   frame->GetXaxis()->SetTimeDisplay(1);
//   frame->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
//   frame->GetXaxis()->SetTimeOffset(0);
//   frame->GetYaxis()->SetTitle("anode factor");
//   frame->SetNdivisions(-404);
//   frame->Draw();

//   lifetimeData->Draw("AnoF:datime.Convert()","","SAME");

//   TString sAtM = lifetimeFile[13];

//   TString lifetimeImage = lifetimeFile.ReplaceAll(".txt",".ps");
//   TString lifetimeImagePNG = lifetimeFile.ReplaceAll(".ps",".png");

//   c1->Print(lifetimeImage);
//   std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
//   gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);







}
