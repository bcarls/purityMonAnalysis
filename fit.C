#include "TGraph.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TComplex.h"
#include "TFFTRealComplex.h"
#include "TFFTComplexReal.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

double func(double *x, double *par){
  if (x[0]<par[1]) return par[3];
  else if (x[0]<par[2]) return par[0]*(1-exp(-(x[0]-par[1])/par[4]))+par[3];
  else return par[0]*(1-exp(-(par[2]-par[1])/par[4]))*exp(-(x[0]-par[2])/par[4])+par[3];
}

void fit(){

  vector<vector<double> > vx(10);
  vector<vector<double> > vy(10);
  ifstream in;
  //in.open("Run_000895_01.txt");
  //in.open("Run_000894_01.txt");
  in.open("Run_000896_01.txt");
  char line[1024];

  int i = 0;
  while(1){
    in.getline(line,1024);
    if (!in.good()) break;
    if (i){
      int n = (i-1)/5008;
      double x, y;
      sscanf(line,"%lf %*s %lf %*s",&x,&y);
      //cout<<n<<" "<<x<<" "<<y<<endl;
      vx[n].push_back(x);
      vy[n].push_back(y);
    }
    ++i;
  }

  TF1 *fun = new TF1("func",func,0,5010,5);
  fun->SetParameters(-0.015,0.0005,0.00062, 0, 1.3e-4);

  vector<double> ey;
  ey.resize(vx[0].size());
  for (size_t i = 0; i<ey.size(); ++i){
    ey[i] = 0.0004;
  }
  
  TGraph *gr[11];
  for (int i = 0; i<11; ++i){
    gr[i] = new TGraphErrors(vx[i].size(),&vx[i][0],&vy[i][0],0,&ey[0]);
  }

  TCanvas *c1 = new TCanvas("c1","c1");
  gr[3]->Draw("ap");
  //fun->Draw("same");
  gr[3]->Fit("func");
  fun->DrawCopy("same");
  double cv0 = fun->GetParameter(0);
  double ct0 = fun->GetParameter(1);
  double ct1 = fun->GetParameter(2);
  double cbaseline = fun->GetParameter(3);
  double crc = fun->GetParameter(4);
  
  double ccorr = (ct1-ct0)/crc/(1-exp(-(ct1-ct0)/crc));
  cout<<ccorr<<endl;
  double vc = -(fun->Eval(ct1)-cbaseline)*ccorr;
  
  TCanvas *c2 = new TCanvas("c2","c2");
  fun->SetParameters(0.008,0.0032,0.0033, 0, 1.3e-4);
  gr[2]->Draw("ap");
  gr[2]->Fit("func");
  fun->DrawCopy("same");
  
  double av0 = fun->GetParameter(0);
  double at0 = fun->GetParameter(1);
  double at1 = fun->GetParameter(2);
  double abaseline = fun->GetParameter(3);
  double arc = fun->GetParameter(4);
  double acorr = (at1-at0)/arc/(1-exp(-(at1-at0)/arc));
  double va = (fun->Eval(at1)-abaseline)*acorr;
  cout<<acorr<<endl;
  cout<<vc<<" "<<va<<endl;
  cout<<(at1-ct1)/log(vc/va)<<endl;
}
