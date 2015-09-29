

// Plot the data coming from the digitizer card reading the purity monitors

// Number of samples
static const int nSamples = 5008;

void plotOscillDataOneTrace(TString PrMFile,TString traceNumber){
  TNtuple *oscillData = new TNtuple("oscillData","NTUPLE","time:anoTrace");
  FILE *fp = fopen(PrMFile.Data(),"r");
  float time, voltage;
  int n = 0;
  int ncols = 0;


  if(atoi(traceNumber.Data()) != 0){
    while (1) {
      ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
      n++;
      if(!(n< 2*atoi(traceNumber.Data())*nSamples))
        break;
    }
    n = 0;
  }

  // Read in time and anode trace
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    if (n < 5)
      std::cout << time << " " << voltage << std::endl;
    oscillData->Fill(time,voltage);
    n++;
    if(!(n<nSamples))
      break;
  }
  // Read in cathode trace
  n=0;
  TBranch *catTrace = oscillData->Branch("catTrace", &voltage, "catTrace/F");
  while (1) {
    ncols = fscanf(fp,"%f sec. %f V",&time,&voltage);
    catTrace->Fill();
    n++;
    if(!(n<nSamples))
      break;
  }









  //Find minimum and maximum voltages, needed to get range right for plotting
  int nEntries = (int)oscillData->GetEntries();
  float anoTraceVolt, catTraceVolt, timeToHist;
  float minVoltage=999999, maxVoltage=-999999;
  oscillData->SetBranchAddress("time",&timeToHist);
  oscillData->SetBranchAddress("anoTrace",&anoTraceVolt);
  oscillData->SetBranchAddress("catTrace",&catTraceVolt);
  for(int i = 0; i < nEntries; i++){
    voltage = oscillData->GetEntry(i);
    if(anoTraceVolt < minVoltage)
      minVoltage=anoTraceVolt;
    if(catTraceVolt < minVoltage)
      minVoltage=catTraceVolt;
    if(anoTraceVolt > maxVoltage)
      maxVoltage=anoTraceVolt;
    if(catTraceVolt > maxVoltage)
      maxVoltage=catTraceVolt;


  }



  /*Plot the oscilloscope scans*/
  TCanvas *c1 = new TCanvas("c1","Tree test");
  TH2F *frame = new TH2F("frame","", 100, 1000*(oscillData->GetMinimum("time")), 1000*(oscillData->GetMaximum("time")), 100, 1000*(minVoltage-0.1*fabs(maxVoltage-minVoltage)), 1000*(maxVoltage+0.5*fabs(maxVoltage-minVoltage)));
  gStyle->SetOptStat(0);
  frame->Draw();
  leg = new TLegend(0.1,0.7,0.48,0.9);
  leg->SetFillColor(0);
  oscillData->SetMarkerStyle(2);
  oscillData->SetMarkerColor(2);
  oscillData->SetLineColor(2);
  oscillData->SetLineWidth(4);
  oscillData->SetLineStyle(2);
  oscillData->SetMarkerSize(0.5);
  oscillData->Draw("1000*(catTrace):1000*time","","LSAME");
  TGraph *graph = (TGraph*)gPad->GetPrimitive("Graph");
  frame->GetXaxis()->SetTitle("ms");
  frame->GetYaxis()->SetTitle("mV");
  leg->AddEntry(graph,"cathode trace","l");
  oscillData->SetMarkerStyle(5);
  oscillData->SetMarkerColor(4);
  oscillData->SetLineColor(4);
  oscillData->SetLineStyle(1);
  oscillData->SetLineWidth(4);
  oscillData->Draw("1000*(anoTrace):1000*time","","LSAME");
  TGraph *graph1 = (TGraph*)gPad->GetPrimitive("Graph")->Clone();
  graph1->SetMarkerStyle(5);
  graph1->SetMarkerColor(4);
  graph1->SetLineColor(4);
  graph1->SetLineWidth(4);
  leg->AddEntry(graph1,"anode trace","l");

  leg->Draw();





















}
