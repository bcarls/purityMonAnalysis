
import ROOT
import sys

ROOT.gROOT.ProcessLine(".L lifetimeCalc.cxx")
ROOT.gROOT.ProcessLine(".L plotAnodeCathodeDiff.cxx")

p = ROOT.plotAnodeCathodeDiff()

p.RunPlotAndLifetime("Run_000801_01.txt")


