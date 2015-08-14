
void plotAveragedLifetime(TString lifetimeFile = "lifetimes_01.txt" ){

  
  TNtuple *lifetimeData = new TNtuple("lifetimeData","NTUPLE","run:month:day:year:hour:minute:second:lifetime:QC:QA"); 
  lifetimeData->ReadFile(lifetimeFile.Data());
  TNtuple *averagedLifetimeData = new TNtuple("averagedLifetime","NTUPLE","averagedRun:averagedLifetime:datime");

  float runNumber, Lifetime, QC, QA;
  float runNumberSum, LifetimeSum, datimeSum;
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


  TDatime datime, datimeMin, datimeMax;
  TBranch *datimeBranch = lifetimeData->Branch("datime", &datime);
  float lifetimeValue;
  float lifetimeMax = 0;
  for(int i = 0; i < lifetimeData->GetEntries(); i++){
    lifetimeData->GetEntry(i);
    if(i == 0)
      datimeMin.Set(year,month,day,hour,minute,second);
    if(i == lifetimeData->GetEntries()-1)
      datimeMax.Set(year,month,day,hour,minute,second);
    datime.Set(year,month,day,hour,minute,second);
    datimeBranch->Fill();
    if(lifetimeValue > lifetimeMax && lifetimeValue < 0.1)
      lifetimeMax = lifetimeValue;
  }
  
  
  
  
  int status;
  int j=0;
  int measPerAve = 6;
  LifetimeSum = 0;
  datimeSum = 0;
  for(int i = 0; i < nEntries; i++){
    status = lifetimeData->GetEntry(i);
    std::cout << runNumber << " " << Lifetime << std::endl;
    runNumberSum+=runNumber;
    LifetimeSum+=Lifetime;
    datimeSum+=datime.Convert();
    j++;
    if(j==measPerAve){
      averagedLifetimeData->Fill(runNumberSum/measPerAve,LifetimeSum/measPerAve,datimeSum/measPerAve);
      std::cout << "average: " << datimeSum/(double)measPerAve << std::endl;
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
    std::cout << Lifetime << " " << averagedLifetimeValue << std::endl;
    lifetimeStandardDevValue += pow(Lifetime-averagedLifetimeValue,2);
    std::cout << "lifetimeStandardDevValue" << lifetimeStandardDevValue << std::endl;
    j++;
    if(j==measPerAve){
      lifetimeStandardDevValue = sqrt((1/(double)measPerAve)*lifetimeStandardDevValue);
      std::cout << "SD: " << lifetimeStandardDevValue << std::endl;
      lifetimeStandardDev->Fill();
      lifetimeStandardDevValue=0; 
      j=0;
    }
  }
  if(j!=0){
    lifetimeStandardDevValue = sqrt((1/(double)j)*lifetimeStandardDevValue);
    std::cout << "SD: " << lifetimeStandardDevValue << std::endl;
    lifetimeStandardDev->Fill();
  }






  TCanvas *c1 = new TCanvas("c1","Tree test");
  c1->SetGridx(1);
  c1->SetGridy(1);
  gStyle->SetOptStat(0);
  TH2F *frame = new TH2F("frame","", 1000, averagedLifetimeData->GetMinimum("averagedRun")-10, averagedLifetimeData->GetMaximum("averagedRun")+10, 1000, 0, 1.1*1000*averagedLifetimeData->GetMaximum("averagedLifetime"));
  frame->GetXaxis()->SetTitle("date/time");
  frame->GetXaxis()->SetTimeDisplay(1);
  frame->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  frame->GetXaxis()->SetTimeOffset(0);
  frame->Draw();

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
  gr->GetXaxis()->SetNdivisions(-404);



  gr->Draw("APESAME");









//   averagedLifetimeData->SetMarkerStyle(2);
//   averagedLifetimeData->SetMarkerColor(2);
//   averagedLifetimeData->SetLineColor(2);
//   averagedLifetimeData->SetMarkerSize(3.0);


//   TCanvas *c1 = new TCanvas("c1","Tree test");
//   // c1->SetGridx(1);
//   // c1->SetGridy(1);
//   gStyle->SetOptStat(0);
//   TH2F *frame = new TH2F("frame","", 1000, averagedLifetimeData->GetMinimum("averagedRun")-10, averagedLifetimeData->GetMaximum("averagedRun")+10, 1000, 0, 1.1*1000*averagedLifetimeData->GetMaximum("averagedLifetime"));
//   frame->GetXaxis()->SetTitle("averaged run number");
//   frame->GetYaxis()->SetTitle("averaged lifetime (ms)");
//   frame->Draw();

//   averagedLifetimeData->Draw("1000*averagedLifetime:averagedRun","","SAME");

//   TString sAtM = lifetimeFile[13];

//   TString lifetimeImage = lifetimeFile.ReplaceAll(".txt",".ps");
//   TString lifetimeImagePNG = lifetimeFile.ReplaceAll(".ps",".png");
//   c1->Print(lifetimeImage.ReplaceAll("lifetime","averagedLifetime"));
//   std::cout << "convert -rotate 90 "+lifetimeImage.ReplaceAll("lifetime","averagedLifetime")+" "+lifetimeImagePNG.ReplaceAll("lifetime","averagedLifetime") << std::endl;
//   gSystem->Exec("convert -rotate 90 "+lifetimeImage.ReplaceAll("lifetime","averagedLifetime")+" "+lifetimeImagePNG.ReplaceAll("lifetime","averagedLifetime"));

}

