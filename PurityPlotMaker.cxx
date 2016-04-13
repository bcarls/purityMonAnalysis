
#include "PurityPlotMaker.h"
#include "root/TNtuple.h"

void PurityPlotMaker::MakePlots(){
  TNtuple *lifetimeData = new TNtuple("lifetimeData","NTUPLE","month:day:year:hour:minute:second:lifetime:QC:QA:CatBase:AnoBase:CathF:AnoF");
  TNtuple *averagedLifetimeData = new TNtuple("averagedLifetime","NTUPLE","averagedDatime:averagedLifetime");

  for(int i = 0; i < listOfDataFiles.size(); i++)
    lifetimeData->ReadFile(listOfDataFiles[i].Data());

  // For publication 
  // TString dataQualityCuts = "lifetime<0.1 && CatRMS < 0.00005 && AnoRMS<0.00012 && AnoF < 1.4 && AnoF > 1.1";
  // For live plots
  // TString dataQualityCuts = "lifetime<0.1 && CatRMS < 0.0005 && CatBase > -0.001 && CatBase < 0.001 && AnoRMS < 0.0001";
  
  // Loose cuts
  TString dataQualityCuts = "lifetime<0.1";


  float Lifetime, QC, QA, CatBase, AnoBase, CathF, AnoF;
  // float runNumberSum, LifetimeSum, QAQCSum;
  float datimeSum, LifetimeSum, QAQCSum;
  float month, day, year, hour, minute, second;
  int nEntries = (int)lifetimeData->GetEntries();
  // lifetimeData->SetBranchAddress("run",&runNumber);
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
  // lifetimeData->SetBranchAddress("CatRMS",&CatRMS);
  // lifetimeData->SetBranchAddress("AnoRMS",&AnoRMS);
  lifetimeData->SetBranchAddress("CathF",&CathF);
  lifetimeData->SetBranchAddress("AnoF",&AnoF);

  TDatime datime, datimeMin, datimeMax;
  UInt_t datimeMinConvert = 1739935037;
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



  TString sAtM = listOfDataFiles[0][24];



  // Do the averaging
  int status;
  int j=0;
  int measPerAve = 6;
  LifetimeSum = 0;
  QAQCSum = 0;
  int QAQCSumCount = 0;
  float datimeSum = 0;
  // Sort the TTree so that the average actually makes sense
  lifetimeData->BuildIndex("datime.Convert()");
  TTreeIndex *lifetimeDataIndex = (TTreeIndex*)lifetimeData->GetTreeIndex();
  for(int i = 0; i < lifetimeDataIndex->GetN(); i++){
    Long64_t local = lifetimeData->LoadTree(lifetimeDataIndex->GetIndex()[i]);
    lifetimeData->GetEntry(local);
    if(lifetimeValue >= 0.1)
      continue;
    if(CatBase <= -0.001)
      continue;
    if(CatBase >= 0.001)
      continue;
    // runNumberSum+=runNumber;
    LifetimeSum+=lifetimeValue;
    QAQCSum += QA/QC;
    QAQCSumCount++;
    datimeSum+=datime.Convert();
    j++;
    if(j==measPerAve){
      // std::cout << LifetimeSum/measPerAve << std::endl;
      averagedLifetimeData->Fill(datimeSum/measPerAve,LifetimeSum/measPerAve);
      datimeSum=0;
      LifetimeSum=0;
      j=0;
    }
  }
  if(j!=0){
    averagedLifetimeData->Fill(datimeSum/(double)j,LifetimeSum/(double)j);
    // std::cout << LifetimeSum/j << std::endl;
  }
  float QAQCAverage = QAQCSum/QAQCSumCount;
  float QAQCSigma = 0;
  float averagedLifetimeValue;
  float lifetimeAverageUncertValue = 0;
  TBranch *lifetimeAverageUncert = averagedLifetimeData->Branch("lifetimeAverageUncert", &lifetimeAverageUncertValue, "lifetimeAverageUncert/F");
  averagedLifetimeData->SetBranchAddress("averagedLifetime",&averagedLifetimeValue);
  j = 0;
  for(int i = 0; i < lifetimeDataIndex->GetN(); i++){
    Long64_t local = lifetimeData->LoadTree(lifetimeDataIndex->GetIndex()[i]);
    lifetimeData->GetEntry(local);
    status = averagedLifetimeData->GetEntry(i/(double)measPerAve);
    if(lifetimeValue >= 0.1)
      continue;
    // if(CatRMS >= 0.0005)
    //   continue;
    if(CatBase <= -0.001)
      continue;
    if(CatBase >= 0.001)
      continue;
    // if(AnoRMS >= 0.0001)
    //   continue;
    lifetimeAverageUncertValue += pow(lifetimeValue-averagedLifetimeValue,2);
    QAQCSigma += (QAQCAverage-QA/QC)*(QAQCAverage-QA/QC);
    // std::cout << local <<  " " << i/measPerAve << " " << lifetimeValue << " " << local/measPerAve << " " << averagedLifetimeValue << std::endl;
    j++;
    if(j==measPerAve){
      lifetimeAverageUncertValue = sqrt((1/(double)measPerAve)*(1/(double)measPerAve)*lifetimeAverageUncertValue);
      lifetimeAverageUncert->Fill();
      lifetimeAverageUncertValue=0; 
      j=0;
    }
  }
  if(j!=0){
    lifetimeAverageUncertValue = sqrt((1/(double)j)*(1/(double)j)*lifetimeAverageUncertValue);
    lifetimeAverageUncert->Fill();
  }


  QAQCSigma = sqrt(QAQCSigma/QAQCSumCount);

  // std::cout << QAQCAverage << " " << QAQCSigma << " " << std::endl;


  // averagedLifetimeData->Scan();

  // Set custom datimeMin and datimeMax, it's in this format:
  // datimeMin.Set(year,month,day,hour,minute,second);
  TDatime datimePlotBegin, datimePlotEnd,datimePlotFiltBegin;
  // For publication  
  // datimePlotBegin.Set(2015,8,4,0,0,0);
  // datimePlotEnd.Set(2015,8,19,12,0,0);
  // For comparison to cosmic rays
  // datimePlotBegin.Set(2015,8,15,0,0,0);
  // datimePlotEnd.Set(2015,8,31,0,0,0);
  // For Stephen 
  datimePlotBegin.Set(2015,9,16,0,0,0);
  datimePlotEnd.Set(2015,9,23,0,0,0);
  // For live plots
  // datimePlotBegin.Set(datimeMax.Convert()-604800);
  // datimePlotEnd = datimeMax;
  // datimePlotBegin.Set(datimeMin.Convert() - 30*60);
  // datimePlotEnd.Set(datimeMax.Convert() + 30*60);
 
  // The date that filtration began
  datimePlotFiltBegin.Set(2015,7,23,16,0,0);


  datimePlotBegin.Print();
  datimePlotEnd.Print();







  lifetimeData->SetMarkerStyle(8);
  lifetimeData->SetMarkerColor(2);
  lifetimeData->SetLineColor(2);
  lifetimeData->SetMarkerSize(1.0);

  TCanvas *c1 = new TCanvas("c1","Tree test");
  // Turn grid on
  c1->SetGridx(1);
  c1->SetGridy(1);
  // Turn grid off
  c1->SetGridx(0);
  c1->SetGridy(0);
  gStyle->SetOptStat(0);
  gPad->SetBottomMargin(0.125);
  // Set ticks for both sides
  gPad->SetTicks(0,1);



  TH2F *frameLifetime = new TH2F("frameLifetime","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 15);
  frameLifetime->GetXaxis()->SetTitle("Date/Time");
  frameLifetime->GetXaxis()->SetTimeDisplay(1);
  frameLifetime->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameLifetime->GetXaxis()->SetTimeOffset(0);
  frameLifetime->GetXaxis()->SetLabelOffset(0.025);
  frameLifetime->GetXaxis()->SetTitleOffset(1.5);
  frameLifetime->GetYaxis()->SetTitle("Lifetime (ms)");
  // frameLifetime->SetNdivisions(-404);
  frameLifetime->SetNdivisions(-7);
  frameLifetime->Draw();

  lifetimeData->Draw("1000*lifetime:datime.Convert()", dataQualityCuts,"SAME");

  TString lifetimeImage = "lifetime_" + sAtM + ".ps";
  TString lifetimeImagePNG = "lifetime_" + sAtM + ".png";
  TString lifetimeImageMacro = "lifetime_" + sAtM + ".C";

  c1->Print(lifetimeImage);
  c1->Print(lifetimeImageMacro);
  std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);





  // Plot oxygen equivalent contamination
  // Conversion is 
  gPad->SetLogy();
  TH2F *frameContamination = new TH2F("frameContamination","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 1e-2, 1);
  frameContamination->GetXaxis()->SetTitle("Date/Time");
  frameContamination->GetXaxis()->SetTimeDisplay(1);
  frameContamination->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameContamination->GetXaxis()->SetTimeOffset(0);
  frameContamination->GetXaxis()->SetLabelOffset(0.025);
  frameContamination->GetXaxis()->SetTitleOffset(1.5);
  frameContamination->GetYaxis()->SetTitle("Oxygen Equivalent Contamination (ppb)");
  // frameContamination->SetNdivisions(-404);
  frameContamination->SetNdivisions(-7);
  frameContamination->Draw();
  TPaveText *lName = new TPaveText(0.1,0.91,0.3,0.96,"NDC");
  lName->SetFillColor(0);
  lName->AddText("B. Carls 11/21/15");
  lName->Draw("SAME");

  lifetimeData->Draw("1e9*3e-13/lifetime:datime.Convert()", dataQualityCuts,"SAME");

  TString contaminationImage = "contamination_" + sAtM + ".ps";
  TString contaminationImagePNG = "contamination_" + sAtM + ".png";
  TString contaminationImageMacro = "contamination_" + sAtM + ".C";

  c1->Print(contaminationImage);
  c1->Print(contaminationImageMacro);
  std::cout << "convert -rotate 90 "+contaminationImage+" "+contaminationImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 "+contaminationImage+" "+contaminationImagePNG);
  gPad->SetLogy(0);



















  TH2F *frameQAQCuBooNEHalfDrift = new TH2F("frameQAQCuBooNEHalfDrift","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 1);
  frameQAQCuBooNEHalfDrift->GetXaxis()->SetTitle("Date/Time");
  frameQAQCuBooNEHalfDrift->GetXaxis()->SetTimeDisplay(1);
  frameQAQCuBooNEHalfDrift->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameQAQCuBooNEHalfDrift->GetXaxis()->SetTimeOffset(0);
  frameQAQCuBooNEHalfDrift->GetXaxis()->SetLabelOffset(0.025);
  frameQAQCuBooNEHalfDrift->GetXaxis()->SetTitleOffset(1.5);
  frameQAQCuBooNEHalfDrift->GetYaxis()->SetTitle("Q_{A}/Q_{C}");
  // frameQAQCuBooNEHalfDrift->SetNdivisions(-404);
  frameQAQCuBooNEHalfDrift->SetNdivisions(-7);
  frameQAQCuBooNEHalfDrift->Draw();

  lifetimeData->Draw("exp(-1.25/(1000*lifetime)):datime.Convert()",dataQualityCuts,"SAME");

  TString QAQCuBooNEHalfDriftImage = "QAQCuBooNEHalfDrift_" + sAtM + ".ps";
  TString QAQCuBooNEHalfDriftImagePNG = "QAQCuBooNEHalfDrift_" + sAtM + ".png";

  c1->Print(QAQCuBooNEHalfDriftImage);
  std::cout << "convert -rotate 90 "+QAQCuBooNEHalfDriftImage+" "+QAQCuBooNEHalfDriftImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 "+QAQCuBooNEHalfDriftImage+" "+QAQCuBooNEHalfDriftImagePNG);






  TH2F *frameQAQCuBooNEDrift = new TH2F("frameQAQCuBooNEDrift","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 1);
  frameQAQCuBooNEDrift->GetXaxis()->SetTitle("Date/Time");
  frameQAQCuBooNEDrift->GetXaxis()->SetTimeDisplay(1);
  frameQAQCuBooNEDrift->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameQAQCuBooNEDrift->GetXaxis()->SetTimeOffset(0);
  frameQAQCuBooNEDrift->GetXaxis()->SetLabelOffset(0.025);
  frameQAQCuBooNEDrift->GetXaxis()->SetTitleOffset(1.5);
  frameQAQCuBooNEDrift->GetYaxis()->SetTitle("Q_{A}/Q_{C}");
  // frameQAQCuBooNEDrift->SetNdivisions(-404);
  frameQAQCuBooNEDrift->SetNdivisions(-7);
  frameQAQCuBooNEDrift->Draw();

  // This is for the live plots
  lifetimeData->Draw("exp(-2.5/(1000*lifetime)):datime.Convert()",dataQualityCuts,"SAME");

  TString QAQCuBooNEDriftImage = "QAQCuBooNEDrift_" + sAtM + ".ps";
  TString QAQCuBooNEDriftImagePNG = "QAQCuBooNEDrift_" + sAtM + ".png";

  c1->Print(QAQCuBooNEDriftImage);
  std::cout << "convert -rotate 90 "+QAQCuBooNEDriftImage+" "+QAQCuBooNEDriftImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 "+QAQCuBooNEDriftImage+" "+QAQCuBooNEDriftImagePNG);








  // Plot QA/QC
  //
  // A note about uncertainties, I looked at a series of 400 averaged traces.
  // The baseline right in front of the peak is -0.000150951 mV.
  // The peak is at 0.00364704 mV.
  // The baseline way before the peak is nearly flat and is at -0.000393831 mV.
  // The correct is 0.00364704 - - 0.000393831 = 0.004040871.
  // The incorrect with the baseline right in front is 0.00364704 - - 0.000150951 = 0.003821351.
  // The correction factor is 1.057446.
  //
  // The RMS off of the true baseline is 7.33637e-06
  //

  TH2F *frameQAQC = new TH2F("frameQAQC","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert()+3*60*60, 1000, 0, 1);
  frameQAQC->GetXaxis()->SetTitle("Date/Time");
  frameQAQC->GetXaxis()->SetTimeDisplay(1);
  frameQAQC->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  frameQAQC->GetXaxis()->SetTimeOffset(0);
  frameQAQC->GetXaxis()->SetLabelOffset(0.030);
  frameQAQC->GetXaxis()->SetTitleOffset(1.80);
  frameQAQC->GetYaxis()->SetTitle("Q_{A}/Q_{C}");
  frameQAQC->SetNdivisions(-7);
  frameQAQC->Draw();
  // This adds the number of volume exchanges to the top of the plot, we have one volume exchange every 2.5 days. 
  // TDatime's default units is in seconds, that's 1 exchange/(2.5*24*60*60 or 216000 seconds)
  c1->Update();
  // TGaxis *volExQAQC = new TGaxis(gPad->GetUxmin(), gPad->GetUymax(), gPad->GetUxmax(), gPad->GetUymax(), 0,((double)datimePlotEnd.Convert()-(double)datimePlotBegin.Convert())/216000 , 510,"-L");
  TGaxis *volExQAQC = new TGaxis(gPad->GetUxmin(), gPad->GetUymax(), gPad->GetUxmax(), gPad->GetUymax(), ((double)datimePlotBegin.Convert()-(double)datimePlotFiltBegin.Convert())/216000, ((double)datimePlotEnd.Convert()-(double)datimePlotFiltBegin.Convert())/216000, 510,"-L");
  volExQAQC->SetTitle("Volume Exchanges");
  volExQAQC->SetTitleSize(frameQAQC->GetXaxis()->GetTitleSize());
  volExQAQC->SetTitleFont(frameQAQC->GetXaxis()->GetTitleFont());
  volExQAQC->SetTitleOffset(1.2);
  volExQAQC->SetLabelSize(frameQAQC->GetXaxis()->GetLabelSize());
  volExQAQC->SetLabelFont(frameQAQC->GetXaxis()->GetLabelFont());
  volExQAQC->Draw();

  // Data quality cuts for the long monitor
  lifetimeData->Draw("QA/QC:datime.Convert()","","SAME");
 
  // TF1 *f3msLifetime = new TF1("f3msLifetime","[0]",datimeMin.Convert()-50000, datimeMax.Convert()+50000);
  TF1 *f3msLifetime = new TF1("f3msLifetime","[0]",datimePlotBegin.Convert(), datimePlotEnd.Convert());
  double f3msY1 = 0;
  double f3msY2 = 0;
  if(purityMonitor == 0){
    f3msLifetime->SetParameter(0,exp(-0.3/3));
    f3msY1 = 0.910;
    f3msY2 = 0.96;
  }
  if(purityMonitor == 1){
    f3msLifetime->SetParameter(0,exp(-2.82/3));
    f3msY1 = 0.393;
    f3msY2 = 0.443;
  }
  f3msLifetime->SetLineColor(1);
  f3msLifetime->SetLineStyle(2);
  f3msLifetime->Draw("SAME");
  // TPaveText *lab3ms = new TPaveText(datimePlotBegin.Convert()+0.000015e9,f3msY1,datimePlotBegin.Convert()+0.00015e9,f3msY2);
  TPaveText *lab3ms = new TPaveText(0.23,0.43,0.3,0.47,"NDC");
  lab3ms->SetFillColor(0);
  // lab6ms->SetLineColor(0);
  lab3ms->SetFillStyle(4000);
  lab3ms->AddText("3 ms");
  lab3ms->Draw("SAME");

  // TF1 *f6msLifetime = new TF1("f6msLifetime","[0]",datimeMin.Convert()-50000, datimeMax.Convert()+50000);
  TF1 *f6msLifetime = new TF1("f6msLifetime","[0]",datimePlotBegin.Convert(), datimePlotEnd.Convert());
  f6msLifetime->SetParameter(0,exp(-2.82/6)); 
  f6msLifetime->SetLineColor(1);
  f6msLifetime->SetLineStyle(2);
  f6msLifetime->Draw("SAME");
  // TPaveText *lab6ms = new TPaveText(datimePlotBegin.Convert()+0.000015e9,0.633,datimePlotBegin.Convert()+0.00015e9,0.683);
  TPaveText *lab6ms = new TPaveText(0.23,0.615,0.3,0.655,"NDC");
  lab6ms->SetFillColor(0);
  lab6ms->SetFillStyle(4000);
  // lab6ms->SetLineColor(0);
  lab6ms->AddText("6 ms");
  lab6ms->Draw("SAME");
  
  // TF1 *f9msLifetime = new TF1("f9msLifetime","[0]",datimeMin.Convert()-50000, datimeMax.Convert()+50000);
  TF1 *f9msLifetime = new TF1("f9msLifetime","[0]",datimePlotBegin.Convert(), datimePlotEnd.Convert());
  f9msLifetime->SetParameter(0,exp(-2.82/9)); 
  f9msLifetime->SetLineColor(1);
  f9msLifetime->SetLineStyle(2);
  f9msLifetime->Draw("SAME");
  // TPaveText *lab9ms = new TPaveText(datimePlotBegin.Convert()+0.000015e9,0.743,datimePlotBegin.Convert()+0.00015e9,0.793);
  TPaveText *lab9ms = new TPaveText(0.23,0.7,0.3,0.74,"NDC");
  lab9ms->SetFillColor(0);
  lab9ms->SetFillStyle(4000);
  // lab6ms->SetLineColor(0);
  lab9ms->AddText("9 ms");
  lab9ms->Draw("SAME");
  TPaveText *lQAQCName = new TPaveText(0.1,0.95,0.3,1.00,"NDC");
  lQAQCName->SetFillColor(0);
  lQAQCName->AddText("B. Carls 11/21/15");
  lQAQCName->Draw("SAME");

  TString QAQCImage = "QAQC_" + sAtM + ".ps";
  TString QAQCImagePNG = "QAQC_" + sAtM + ".png";
  TString QAQCImageMacro = "QAQC_" + sAtM + ".C";
  c1->Print(QAQCImage);
  c1->Print(QAQCImageMacro);
  std::cout << "convert -rotate 90 " + QAQCImage + " "+ QAQCImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + QAQCImage + " "+ QAQCImagePNG );













  // Plot QA/QC with error bars from baseline calculation
  gStyle->SetOptStat(0);
  // long NQAQC = lifetimeData->Draw("datime.Convert():(1.007)*(1.0)*QA/QC*(run < 2100)+0.9992877*1.057*QA/QC*(run>2113):0.057*QA/QC",dataQualityCuts,"goff");
  long NQAQC = lifetimeData->Draw("datime.Convert():0.9992877*1.057*QA/QC:0.057*QA/QC",dataQualityCuts,"goff");
  TGraphErrors *grQAQC = new TGraphErrors(NQAQC,lifetimeData->GetV1(),lifetimeData->GetV2(),0,lifetimeData->GetV3());
  grQAQC->SetMarkerStyle(8);
  grQAQC->SetMarkerColor(2);
  grQAQC->SetFillColor(kRed);
  grQAQC->SetLineColor(2);
  grQAQC->SetMarkerSize(0.5);
  grQAQC->SetMaximum(1);
  grQAQC->SetMinimum(0);
  grQAQC->GetXaxis()->SetTitle("Date/Time");
  grQAQC->GetYaxis()->SetTitle("Q_{A}/Q_{C}");
  grQAQC->GetXaxis()->SetTimeDisplay(1);
  grQAQC->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  grQAQC->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
  grQAQC->GetXaxis()->SetTimeOffset(0);
  grQAQC->GetXaxis()->SetLabelOffset(0.030);
  grQAQC->GetXaxis()->SetTitleOffset(1.80);
  grQAQC->SetTitle("");
  grQAQC->Draw("AE3");
  grQAQC->GetXaxis()->SetLimits(datimePlotBegin.Convert(),datimePlotEnd.Convert()+3*60*60);
  grQAQC->GetXaxis()->SetNdivisions(-7);
  // This adds the number of volume exchanges to the top of the plot, we have one volume exchange every 2.5 days. 
  // TDatime's default units is in seconds, that's 1 exchange/(2.5*24*60*60 or 216000 seconds)
  c1->Update();
  // TGaxis *volExQAQCError = new TGaxis(gPad->GetUxmin(), gPad->GetUymax(), gPad->GetUxmax(), gPad->GetUymax(), 0,((double)datimePlotEnd.Convert()-(double)datimePlotBegin.Convert())/216000 , 510,"-L");
  TGaxis *volExQAQCError = new TGaxis(gPad->GetUxmin(), gPad->GetUymax(), gPad->GetUxmax(), gPad->GetUymax(), ((double)datimePlotBegin.Convert()-(double)datimePlotFiltBegin.Convert())/216000, ((double)datimePlotEnd.Convert()-(double)datimePlotFiltBegin.Convert())/216000, 510,"-L");
  volExQAQCError->SetTitle("Volume Exchanges");
  volExQAQCError->SetTitleSize(grQAQC->GetXaxis()->GetTitleSize());
  volExQAQCError->SetTitleFont(grQAQC->GetXaxis()->GetTitleFont());
  volExQAQCError->SetTitleOffset(1.2);
  volExQAQCError->SetLabelSize(grQAQC->GetXaxis()->GetLabelSize());
  volExQAQCError->SetLabelFont(grQAQC->GetXaxis()->GetLabelFont());
  volExQAQCError->Draw();

  f3msLifetime->Draw("SAME");
  lab3ms->Draw("SAME");

  if(purityMonitor == 1 || purityMonitor == 2){
    f6msLifetime->Draw("SAME");
    lab6ms->Draw("SAME");
    f9msLifetime->Draw("SAME");
    lab9ms->Draw("SAME");
  }

  TString QAQCErrorImage = "QAQCError_" + sAtM + ".ps";
  TString QAQCErrorImagePNG = "QAQCError_" + sAtM + ".png";
  TString QAQCErrorImageMacro = "QAQCError_" + sAtM + ".C";
  c1->Print(QAQCErrorImage);
  c1->Print(QAQCErrorImageMacro);
  std::cout << "convert -rotate 90 " + QAQCErrorImage + " "+ QAQCErrorImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + QAQCErrorImage + " "+ QAQCErrorImagePNG );




















  // Make the averaged plot
  gStyle->SetOptStat(0);
  long N = averagedLifetimeData->Draw("averagedDatime:1000*averagedLifetime:1000*lifetimeAverageUncert","","goff");
  TGraphErrors *gr = new TGraphErrors(N,averagedLifetimeData->GetV1(),averagedLifetimeData->GetV2(),0,averagedLifetimeData->GetV3());
  gr->SetMarkerStyle(8);
  gr->SetMarkerColor(2);
  gr->SetLineColor(2);
  gr->SetMarkerSize(0.5);
  gr->SetMinimum(0);
  gr->GetXaxis()->SetTitle("averaged run number");
  gr->GetYaxis()->SetTitle("averaged lifetime (ms)");
  gr->GetXaxis()->SetTitle("Date/Time");
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
  lifetimeData->Draw("1000*QA:datime.Convert()",dataQualityCuts,"SAME");

  TString QAImage = "QA_" + sAtM + ".ps";
  TString QAImagePNG = "QA_" + sAtM + ".png";

  c1->Print(QAImage);
  std::cout << "convert -rotate 90 " + QAImage + " "+ QAImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 " + QAImage + " "+ QAImagePNG );









  // TH2F *frameQC = new TH2F("frameQC","", 1000, datimeMin.Convert()-50000, datimeMax.Convert()+50000, 1000, 0, 1);
  TH2F *frameQC = new TH2F("frameQC","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 20);
  frameQC->GetXaxis()->SetTitle("Date/Time");
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



  
  TH2F *frameCatBase = new TH2F("frameCatBase","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, -2, 2);
  frameCatBase->GetXaxis()->SetTitle("Date/Time");
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


 

  TH2F *frameAnoBase = new TH2F("frameAnoBase","", 1000, datimePlotBegin.Convert()-604800, datimePlotEnd.Convert(), 1000, -2, 2);
  frameAnoBase->GetXaxis()->SetTitle("Date/Time");
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










  
//   TH2F *frameCatRMS = new TH2F("frameCatRMS","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 1);
//   frameCatRMS->GetXaxis()->SetTitle("Date/Time");
//   frameCatRMS->GetXaxis()->SetTimeDisplay(1);
//   frameCatRMS->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
//   frameCatRMS->GetXaxis()->SetTimeOffset(0);
//   frameCatRMS->GetXaxis()->SetLabelOffset(0.025);
//   frameCatRMS->GetXaxis()->SetTitleOffset(1.5);
//   frameCatRMS->GetYaxis()->SetTitle("Cathode RMS (mV)");
//   frameCatRMS->SetNdivisions(-7);
//   frameCatRMS->Draw();
//   lifetimeData->Draw("1000*CatRMS:datime.Convert()","","SAME");

//   TString CatRMSImage = "CatRMS_" + sAtM + ".ps";
//   TString CatRMSImagePNG = "CatRMS_" + sAtM + ".png";

//   c1->Print(CatRMSImage);
//   std::cout << "convert -rotate 90 " + CatRMSImage + " "+ CatRMSImagePNG << std::endl;
//   gSystem->Exec("convert -rotate 90 " + CatRMSImage + " "+ CatRMSImagePNG );


 

//   TH2F *frameAnoRMS = new TH2F("frameAnoRMS","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 1);
//   frameAnoRMS->GetXaxis()->SetTitle("Date/Time");
//   frameAnoRMS->GetXaxis()->SetTimeDisplay(1);
//   frameAnoRMS->GetXaxis()->SetTimeFormat("#splitline{%m-%d-%y}{%H:%M}");
//   frameAnoRMS->GetXaxis()->SetTimeOffset(0);
//   frameAnoRMS->GetXaxis()->SetLabelOffset(0.025);
//   frameAnoRMS->GetXaxis()->SetTitleOffset(1.5);
//   frameAnoRMS->GetYaxis()->SetTitle("Anode RMS (mV)");
//   frameAnoRMS->SetNdivisions(-7);
//   frameAnoRMS->Draw();
//   lifetimeData->Draw("1000*AnoRMS:datime.Convert()","","SAME");

//   TString AnoRMSImage = "AnoRMS_" + sAtM + ".ps";
//   TString AnoRMSImagePNG = "AnoRMS_" + sAtM + ".png";

//   c1->Print(AnoRMSImage);
//   std::cout << "convert -rotate 90 " + AnoRMSImage + " "+ AnoRMSImagePNG << std::endl;
//   gSystem->Exec("convert -rotate 90 " + AnoRMSImage + " "+ AnoRMSImagePNG );







  
  TH2F *frameCathF = new TH2F("frameCathF","", 1000, datimePlotBegin.Convert(), datimePlotEnd.Convert(), 1000, 0, 10);
  frameCathF->GetXaxis()->SetTitle("Date/Time");
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
  frameAnoF->GetXaxis()->SetTitle("Date/Time");
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

