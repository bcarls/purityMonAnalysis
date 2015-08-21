
void plotLifetime(TString lifetimeFile = "lifetimes_00.txt" ){
  TNtuple *lifetimeData = new TNtuple("lifetimeData","NTUPLE","run:month:day:year:hour:minute:second:lifetime:QC:QA:CatRMS:AnoRMS");
  lifetimeData->ReadFile(lifetimeFile.Data());

  float runNumber, Lifetime, QC, QA, CatRMS, AnoRMS;
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
  lifetimeData->SetBranchAddress("CatRMS",&CatRMS);
  lifetimeData->SetBranchAddress("AnoRMS",&AnoRMS);


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



//   TH2F *frame = new TH2F("frame","", 1000, datimeMin.Convert()-100000, datimeMax.Convert()+100000, 1000, 0, 1.1*1000*lifetimeMax);
//   frame->GetXaxis()->SetTitle("date/time");
//   frame->GetXaxis()->SetTimeDisplay(1);
//   frame->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
//   frame->GetXaxis()->SetTimeOffset(0);
//   frame->GetYaxis()->SetTitle("lifetime (ms)");
//   frame->SetNdivisions(-404);
//   frame->Draw();
//   lifetimeData->Draw("1000*lifetime:datime.Convert()","lifetime<0.1 && AnoRMS < 0.0001","SAME");
//   TString sAtM = lifetimeFile[13];
//   TString lifetimeImage = lifetimeFile.ReplaceAll(".txt",".ps");
//   TString lifetimeImagePNG = lifetimeFile.ReplaceAll(".ps",".png");
//   c1->Print(lifetimeImage);
//   std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
//   gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);

 




//   TH2F *frame = new TH2F("frame","", 1000, datimeMin.Convert()-50000, datimeMax.Convert()+50000, 1000, 0, 1);
//   frame->GetXaxis()->SetTitle("date/time");
//   frame->GetXaxis()->SetTimeDisplay(1);
//   frame->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
//   frame->GetXaxis()->SetTimeOffset(0);
//   frame->GetYaxis()->SetTitle("Q_{A}/Q_{C}");
//   frame->SetNdivisions(-404);
//   frame->Draw();
//   lifetimeData->Draw("QA/QC:datime.Convert()","AnoRMS < 0.0001","SAME");
//   TF1 *f3msLifetime = new TF1("f3msLifetime","[0]",datimeMin.Convert()-50000, datimeMax.Convert()+50000);
//   f3msLifetime->SetParameter(0,exp(-2.82/3)); 
//   f3msLifetime->SetLineColor(1);
//   f3msLifetime->SetLineStyle(2);
//   f3msLifetime->Draw("SAME");
//   TPaveText *lab3ms = new TPaveText(1.43894e9,0.393,1.43903e9,0.443);
//   lab3ms->SetFillColor(0);
//   // lab6ms->SetLineColor(0);
//   lab3ms->AddText("3.0 ms");
//   lab3ms->Draw("SAME");
//   TF1 *f9msLifetime = new TF1("f9msLifetime","[0]",datimeMin.Convert()-50000, datimeMax.Convert()+50000);
//   f9msLifetime->SetParameter(0,exp(-2.82/9)); 
//   f9msLifetime->SetLineColor(1);
//   f9msLifetime->SetLineStyle(2);
//   f9msLifetime->Draw("SAME");
//   TPaveText *lab9ms = new TPaveText(1.43894e9,0.743,1.43903e9,0.793);
//   lab9ms->SetFillColor(0);
//   // lab6ms->SetLineColor(0);
//   lab9ms->AddText("9.0 ms");
//   lab9ms->Draw("SAME");

//   TString sAtM = lifetimeFile[13];

//   TString lifetimeImage = lifetimeFile.ReplaceAll(".txt",".ps");
//   TString lifetimeImagePNG = lifetimeFile.ReplaceAll(".ps",".png");

//   c1->Print(lifetimeImage);
//   std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
//   gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);









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






  TH2F *frame = new TH2F("frame","", 1000, datimeMin.Convert()-2000, datimeMax.Convert()+2000, 1000, 0, 0.001);
  frame->GetXaxis()->SetTitle("date/time");
  frame->GetXaxis()->SetTimeDisplay(1);
  frame->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
  frame->GetXaxis()->SetTimeOffset(0);
  frame->GetYaxis()->SetTitle("anode factor");
  frame->SetNdivisions(-404);
  frame->Draw();

  lifetimeData->Draw("CatRMS:datime.Convert()","","SAME");

  TString sAtM = lifetimeFile[13];

  TString lifetimeImage = lifetimeFile.ReplaceAll(".txt",".ps");
  TString lifetimeImagePNG = lifetimeFile.ReplaceAll(".ps",".png");

  c1->Print(lifetimeImage);
  std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
  gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);






//   TH2F *frame = new TH2F("frame","", 1000, datimeMin.Convert()-2000, datimeMax.Convert()+2000, 1000, 0, 0.001);
//   frame->GetXaxis()->SetTitle("date/time");
//   frame->GetXaxis()->SetTimeDisplay(1);
//   frame->GetXaxis()->SetTimeFormat("%Y-%m-%d %H:%M");
//   frame->GetXaxis()->SetTimeOffset(0);
//   frame->GetYaxis()->SetTitle("anode factor");
//   frame->SetNdivisions(-404);
//   frame->Draw();

//   lifetimeData->Draw("AnoRMS:datime.Convert()","","SAME");

//   TString sAtM = lifetimeFile[13];

//   TString lifetimeImage = lifetimeFile.ReplaceAll(".txt",".ps");
//   TString lifetimeImagePNG = lifetimeFile.ReplaceAll(".ps",".png");

//   c1->Print(lifetimeImage);
//   std::cout << "convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG << std::endl;
//   gSystem->Exec("convert -rotate 90 "+lifetimeImage+" "+lifetimeImagePNG);





}

