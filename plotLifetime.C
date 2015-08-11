
void plotLifetime(TString lifetimeFile = "lifetimes_00.txt" ){
  TNtuple *lifetimeData = new TNtuple("lifetimeData","NTUPLE","run:month:day:year:hour:minute:second:lifetime");
  /*TNtuple *averagedLifetimeData = new TNtuple("averagedLifetime","NTUPLE","averagedRun:averagedLifetime");*/
  lifetimeData->ReadFile(lifetimeFile.Data());

  float runNumber, Lifetime;
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


  TDatime datime, datimeMin, datimeMax;
  TBranch *datimeBranch = lifetimeData->Branch("datime", &datime);
  float lifetimeValue;
  float lifetimeMax = 0;
  lifetimeData->SetBranchAddress("lifetime",&lifetimeValue);
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

  datimeMin.Print();
  datimeMax.Print();




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



  


  TH2F *frame = new TH2F("frame","", 1000, datimeMin.Convert()-2000, datimeMax.Convert()+2000, 1000, 0, 1.1*1000*lifetimeMax);
  frame->GetXaxis()->SetTitle("date/time");
  frame->GetXaxis()->SetTimeDisplay(1);
  frame->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  frame->GetXaxis()->SetTimeOffset(0);
  frame->GetYaxis()->SetTitle("lifetime (ms)");
  frame->SetNdivisions(-404);
  frame->Draw();

  lifetimeData->Draw("1000*lifetime:datime.Convert()","lifetime<0.1","SAME");

  TString sAtM = lifetimeFile[13];

  TString lifetimeImage = lifetimeFile.ReplaceAll(".txt",".ps");
  TString lifetimeImagePNG = lifetimeFile.ReplaceAll(".ps",".png");

  c1->Print(lifetimeImage);
  std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);

}

