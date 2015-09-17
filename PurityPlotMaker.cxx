

#include "PurityPlotMaker.h"



void PurityPlotMaker::MakePlots(){
  TNtuple *lifetimeData = new TNtuple("lifetimeData","NTUPLE","run:month:day:year:hour:minute:second:lifetime:QC:QA:CatBase:AnoBase:CatRMS:AnoRMS:CathF:AnoF");
  TNtuple *averagedLifetimeData = new TNtuple("averagedLifetime","NTUPLE","averagedRun:averagedLifetime:datime");

  for(int i = 0; i < listOfDataFiles.size(); i++)
    lifetimeData->ReadFile(listOfDataFiles[i].Data());

  // lifetimeData->Scan();

  float runNumber, Lifetime, QC, QA, CatBase, AnoBase, CatRMS, AnoRMS, CathF, AnoF;
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
  // lifetimeData->SetBranchAddress("lifetime",&Lifetime);
  lifetimeData->SetBranchAddress("QC",&QC);
  lifetimeData->SetBranchAddress("QA",&QA);
  lifetimeData->SetBranchAddress("CatBase",&CatBase);
  lifetimeData->SetBranchAddress("AnoBase",&AnoBase);
  lifetimeData->SetBranchAddress("CatRMS",&CatRMS);
  lifetimeData->SetBranchAddress("AnoRMS",&AnoRMS);
  lifetimeData->SetBranchAddress("CathF",&CathF);
  lifetimeData->SetBranchAddress("AnoF",&AnoF);

  TDatime datime, datimeMin, datimeMax;
  UInt_t datimeMinConvert = 1739935037;
  int datimeMaxConvert = 0;
  TBranch *datimeBranch = lifetimeData->Branch("datime", &datime);
  float lifetimeValue;
  float lifetimeMax = 0;
  float QAMax = 0;
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

    if(QA > QAMax)
      QAMax = QA;

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
  // Sort the TTree so that the average actually makes sense
  lifetimeData->BuildIndex("run");
  TTreeIndex *lifetimeDataIndex = (TTreeIndex*)lifetimeData->GetTreeIndex();
  // for(int i = lifetimeDataIndex->GetN() - 1; i >= 0; --i){
  for(int i = 0; i < lifetimeDataIndex->GetN(); i++){
    Long64_t local = lifetimeData->LoadTree(lifetimeDataIndex->GetIndex()[i]);
    lifetimeData->GetEntry(local);
    // std::cout << runNumber << " " << lifetimeValue << std::endl;
    if(lifetimeValue >= 0.1)
      continue;
    if(CatRMS >= 0.00005)
      continue;
    if(AnoRMS >= 0.00012)
      continue;
    if(AnoF <= 1.1)
      continue;
    if(AnoF >= 1.4)
      continue;
    runNumberSum+=runNumber;
    LifetimeSum+=lifetimeValue;
    datimeSum+=datime.Convert();
    // std::cout << j << " " << lifetimeValue << std::endl;
    j++;
    if(j==measPerAve){
      averagedLifetimeData->Fill(runNumberSum/measPerAve,LifetimeSum/measPerAve,datimeSum/measPerAve);
      // std::cout << LifetimeSum/measPerAve << std::endl;
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
  for(int i = 0; i < lifetimeDataIndex->GetN(); i++){
    Long64_t local = lifetimeData->LoadTree(lifetimeDataIndex->GetIndex()[i]);
    lifetimeData->GetEntry(local);
    status = averagedLifetimeData->GetEntry(i/measPerAve);
    // std::cout << i <<  " " << lifetimeValue << " " << i/measPerAve << " " << averagedLifetimeValue << std::endl;
    if(lifetimeValue >= 0.1)
      continue;
    if(CatRMS >= 0.00005)
      continue;
    if(AnoRMS >= 0.00012)
      continue;
    if(AnoF <= 1.1)
      continue;
    if(AnoF >= 1.4)
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



  // Set custom datimeMin and datimeMax, it's in this format:
  // datimeMin.Set(year,month,day,hour,minute,second);
  TDatime datimePlotBegin, datimePlotEnd;
  // datimePlotBegin.Set(2015,8,4,0,0,0);
  // datimePlotEnd.Set(2015,8,19,12,0,0);
  datimePlotBegin.Set(datimeMax.Convert()-604800);
  datimePlotEnd = datimeMax;









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


  TH2F *frameLifetime = new TH2F("frameLifetime","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 15);
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

  lifetimeData->Draw("1000*lifetime:datime.Convert()","lifetime<0.1 && CatRMS < 0.00005 && AnoRMS<0.00012 && AnoF < 1.4 && AnoF > 1.1","SAME");

  TString lifetimeImage = "lifetime_" + sAtM + ".ps";
  TString lifetimeImagePNG = "lifetime_" + sAtM + ".png";

  c1->Print(lifetimeImage);
  std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);

 








  // TH2F *frameQAQC = new TH2F("frameQAQC","", 1000, datimeMax.Convert()-604800, datimeMax.Convert(), 1000, 0, 1);
  TH2F *frameQAQC = new TH2F("frameQAQC","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 1);
  frameQAQC->GetXaxis()->SetTitle("date/time");
  frameQAQC->GetXaxis()->SetTimeDisplay(1);
  frameQAQC->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameQAQC->GetXaxis()->SetTimeOffset(0);
  frameQAQC->GetXaxis()->SetLabelOffset(0.030);
  frameQAQC->GetXaxis()->SetTitleOffset(1.55);
  frameQAQC->GetYaxis()->SetTitle("Q_{A}/Q_{C}");
  frameQAQC->SetNdivisions(-7);
  frameQAQC->Draw();
  lifetimeData->Draw("QA/QC:datime.Convert()","lifetime<0.1 && CatRMS < 0.00005 && AnoRMS<0.00012 && AnoF < 1.4 && AnoF > 1.1","SAME");
  // TF1 *f3msLifetime = new TF1("f3msLifetime","[0]",datimeMin.Convert()-50000, datimeMax.Convert()+50000);
  TF1 *f3msLifetime = new TF1("f3msLifetime","[0]",datimePlotBegin.Convert(), datimePlotEnd.Convert());
  f3msLifetime->SetParameter(0,exp(-2.82/3)); 
  f3msLifetime->SetLineColor(1);
  f3msLifetime->SetLineStyle(2);
  f3msLifetime->Draw("SAME");
  // TPaveText *lab3ms = new TPaveText(1.43894e9,0.393,1.43903e9,0.443);
  TPaveText *lab3ms = new TPaveText(datimePlotBegin.Convert()+0.00001e9,0.393,datimePlotBegin.Convert()+0.0001e9,0.443);
  lab3ms->SetFillColor(0);
  // lab6ms->SetLineColor(0);
  lab3ms->AddText("3 ms");
  lab3ms->Draw("SAME");
  // TF1 *f9msLifetime = new TF1("f9msLifetime","[0]",datimeMin.Convert()-50000, datimeMax.Convert()+50000);
  TF1 *f9msLifetime = new TF1("f9msLifetime","[0]",datimePlotBegin.Convert(), datimePlotEnd.Convert());
  f9msLifetime->SetParameter(0,exp(-2.82/9)); 
  f9msLifetime->SetLineColor(1);
  f9msLifetime->SetLineStyle(2);
  f9msLifetime->Draw("SAME");
  // TPaveText *lab9ms = new TPaveText(1.43894e9,0.743,1.43903e9,0.793);
  TPaveText *lab9ms = new TPaveText(datimePlotBegin.Convert()+0.00001e9,0.743,datimePlotBegin.Convert()+0.0001e9,0.793);
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
  gr->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  gr->GetXaxis()->SetTimeOffset(0);
  gr->GetXaxis()->SetLabelOffset(0.025);
  gr->GetXaxis()->SetTitleOffset(1.5);
  gr->Draw("APE");
  // gr->GetXaxis()->SetRange(datimeMax.Convert()-604800,datimeMax.Convert());
  gr->GetXaxis()->SetLimits(datimePlotBegin.Convert(),datimePlotEnd.Convert());
  gr->GetXaxis()->SetNdivisions(-7);

  TString averageLifetimeImage = "averageLifetime_" + sAtM + ".ps";
  TString averageLifetimeImagePNG = "averageLifetime_" + sAtM + ".png";

  c1->Print(averageLifetimeImage);
  std::cout << "convert -rotate 90 "+averageLifetimeImage+" "+averageLifetimeImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 "+averageLifetimeImage+" "+averageLifetimeImagePNG);











  // TH2F *frameQAQC = new TH2F("frameQAQC","", 1000, datimeMin.Convert()-50000, datimeMax.Convert()+50000, 1000, 0, 1);
  TH2F *frameQA = new TH2F("frameQA","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 20);
  frameQA->GetXaxis()->SetTitle("date/time");
  frameQA->GetXaxis()->SetTimeDisplay(1);
  frameQA->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameQA->GetXaxis()->SetTimeOffset(0);
  frameQA->GetXaxis()->SetLabelOffset(0.025);
  frameQA->GetXaxis()->SetTitleOffset(1.5);
  frameQA->GetYaxis()->SetTitle("Q_{A} (mV)");
  frameQA->SetNdivisions(-7);
  frameQA->Draw();
  lifetimeData->Draw("1000*QA:datime.Convert()","","SAME");

  TString QAImage = "QA_" + sAtM + ".ps";
  TString QAImagePNG = "QA_" + sAtM + ".png";

  c1->Print(QAImage);
  std::cout << "convert -rotate 90 " + QAImage + " "+ QAImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + QAImage + " "+ QAImagePNG );









  // TH2F *frameQC = new TH2F("frameQC","", 1000, datimeMin.Convert()-50000, datimeMax.Convert()+50000, 1000, 0, 1);
  TH2F *frameQC = new TH2F("frameQC","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 20);
  frameQC->GetXaxis()->SetTitle("date/time");
  frameQC->GetXaxis()->SetTimeDisplay(1);
  frameQC->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameQC->GetXaxis()->SetTimeOffset(0);
  frameQC->GetXaxis()->SetLabelOffset(0.025);
  frameQC->GetXaxis()->SetTitleOffset(1.5);
  frameQC->GetYaxis()->SetTitle("Q_{C} (mV)");
  frameQC->SetNdivisions(-7);
  frameQC->Draw();
  lifetimeData->Draw("1000*QC:datime.Convert()","","SAME");

  TString QCImage = "QC_" + sAtM + ".ps";
  TString QCImagePNG = "QC_" + sAtM + ".png";

  c1->Print(QCImage);
  std::cout << "convert -rotate 90 " + QCImage + " "+ QCImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + QCImage + " "+ QCImagePNG );



  
  TH2F *frameCatBase = new TH2F("frameCatBase","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, -1, 1);
  frameCatBase->GetXaxis()->SetTitle("date/time");
  frameCatBase->GetXaxis()->SetTimeDisplay(1);
  frameCatBase->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameCatBase->GetXaxis()->SetTimeOffset(0);
  frameCatBase->GetXaxis()->SetLabelOffset(0.025);
  frameCatBase->GetXaxis()->SetTitleOffset(1.5);
  frameCatBase->GetYaxis()->SetTitle("Cathode Base (mV)");
  frameCatBase->SetNdivisions(-7);
  frameCatBase->Draw();
  lifetimeData->Draw("1000*CatBase:datime.Convert()","","SAME");

  TString CatBaseImage = "CatBase_" + sAtM + ".ps";
  TString CatBaseImagePNG = "CatBase_" + sAtM + ".png";

  c1->Print(CatBaseImage);
  std::cout << "convert -rotate 90 " + CatBaseImage + " "+ CatBaseImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + CatBaseImage + " "+ CatBaseImagePNG );


 

  TH2F *frameAnoBase = new TH2F("frameAnoBase","", 1000, datimePlotBegin.Convert()-604800, datimePlotEnd.Convert(), 1000, -1, 1);
  frameAnoBase->GetXaxis()->SetTitle("date/time");
  frameAnoBase->GetXaxis()->SetTimeDisplay(1);
  frameAnoBase->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameAnoBase->GetXaxis()->SetTimeOffset(0);
  frameAnoBase->GetXaxis()->SetLabelOffset(0.025);
  frameAnoBase->GetXaxis()->SetTitleOffset(1.5);
  frameAnoBase->GetYaxis()->SetTitle("Anode Base (mV)");
  frameAnoBase->SetNdivisions(-7);
  frameAnoBase->Draw();
  lifetimeData->Draw("1000*AnoBase:datime.Convert()","","SAME");

  TString AnoBaseImage = "AnoBase_" + sAtM + ".ps";
  TString AnoBaseImagePNG = "AnoBase_" + sAtM + ".png";

  c1->Print(AnoBaseImage);
  std::cout << "convert -rotate 90 " + AnoBaseImage + " "+ AnoBaseImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + AnoBaseImage + " "+ AnoBaseImagePNG );










  
  TH2F *frameCatRMS = new TH2F("frameCatRMS","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 0.2);
  frameCatRMS->GetXaxis()->SetTitle("date/time");
  frameCatRMS->GetXaxis()->SetTimeDisplay(1);
  frameCatRMS->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameCatRMS->GetXaxis()->SetTimeOffset(0);
  frameCatRMS->GetXaxis()->SetLabelOffset(0.025);
  frameCatRMS->GetXaxis()->SetTitleOffset(1.5);
  frameCatRMS->GetYaxis()->SetTitle("Cathode RMS (mV)");
  frameCatRMS->SetNdivisions(-7);
  frameCatRMS->Draw();
  lifetimeData->Draw("1000*CatRMS:datime.Convert()","","SAME");

  TString CatRMSImage = "CatRMS_" + sAtM + ".ps";
  TString CatRMSImagePNG = "CatRMS_" + sAtM + ".png";

  c1->Print(CatRMSImage);
  std::cout << "convert -rotate 90 " + CatRMSImage + " "+ CatRMSImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + CatRMSImage + " "+ CatRMSImagePNG );


 

  TH2F *frameAnoRMS = new TH2F("frameAnoRMS","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 1);
  frameAnoRMS->GetXaxis()->SetTitle("date/time");
  frameAnoRMS->GetXaxis()->SetTimeDisplay(1);
  frameAnoRMS->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameAnoRMS->GetXaxis()->SetTimeOffset(0);
  frameAnoRMS->GetXaxis()->SetLabelOffset(0.025);
  frameAnoRMS->GetXaxis()->SetTitleOffset(1.5);
  frameAnoRMS->GetYaxis()->SetTitle("Anode RMS (mV)");
  frameAnoRMS->SetNdivisions(-7);
  frameAnoRMS->Draw();
  lifetimeData->Draw("1000*AnoRMS:datime.Convert()","","SAME");

  TString AnoRMSImage = "AnoRMS_" + sAtM + ".ps";
  TString AnoRMSImagePNG = "AnoRMS_" + sAtM + ".png";

  c1->Print(AnoRMSImage);
  std::cout << "convert -rotate 90 " + AnoRMSImage + " "+ AnoRMSImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + AnoRMSImage + " "+ AnoRMSImagePNG );







  
  TH2F *frameCathF = new TH2F("frameCathF","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 10);
  frameCathF->GetXaxis()->SetTitle("date/time");
  frameCathF->GetXaxis()->SetTimeDisplay(1);
  frameCathF->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameCathF->GetXaxis()->SetTimeOffset(0);
  frameCathF->GetXaxis()->SetLabelOffset(0.025);
  frameCathF->GetXaxis()->SetTitleOffset(1.5);
  frameCathF->GetYaxis()->SetTitle("Cathhode F (mV)");
  frameCathF->SetNdivisions(-7);
  frameCathF->Draw();
  lifetimeData->Draw("CathF:datime.Convert()","","SAME");

  TString CathFImage = "CathF_" + sAtM + ".ps";
  TString CathFImagePNG = "CathF_" + sAtM + ".png";

  c1->Print(CathFImage);
  std::cout << "convert -rotate 90 " + CathFImage + " "+ CathFImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + CathFImage + " "+ CathFImagePNG );


 

  TH2F *frameAnoF = new TH2F("frameAnoF","", 1000, datimePlotBegin.Convert()-604800, datimePlotEnd.Convert(), 1000, 0, 10);
  frameAnoF->GetXaxis()->SetTitle("date/time");
  frameAnoF->GetXaxis()->SetTimeDisplay(1);
  frameAnoF->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameAnoF->GetXaxis()->SetTimeOffset(0);
  frameAnoF->GetXaxis()->SetLabelOffset(0.025);
  frameAnoF->GetXaxis()->SetTitleOffset(1.5);
  frameAnoF->GetYaxis()->SetTitle("Anode F (mV)");
  frameAnoF->SetNdivisions(-7);
  frameAnoF->Draw();
  lifetimeData->Draw("AnoF:datime.Convert()","","SAME");

  TString AnoFImage = "AnoF_" + sAtM + ".ps";
  TString AnoFImagePNG = "AnoF_" + sAtM + ".png";

  c1->Print(AnoFImage);
  std::cout << "convert -rotate 90 " + AnoFImage + " "+ AnoFImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + AnoFImage + " "+ AnoFImagePNG );







}

