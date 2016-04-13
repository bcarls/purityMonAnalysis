import ROOT


c1 = ROOT.TCanvas("c1","Tree test")
# c1.SetLogy()

fLifetimeContamination = ROOT.TF1("fLifetimeContamination","[0]/x",0, 10)

# The 3e-13 is in usned of parts per oxygen equivalent, lifetime is in seconds
# contaminant = 3e-13 sec. pp oxygen / lifetime sec
# lifetime = 3e-13 sec. pp oxygen /contaminant pp oxygen

# fLifetimeContamination.SetParameter(0,3e-13) 
# Convert seconds to ms and concentration to ppt
fLifetimeContamination.SetParameter(0,3e2) 
fLifetimeContamination.SetLineColor(2)
fLifetimeContamination.SetLineStyle(0)
fLifetimeContamination.SetMaximum(1000)
fLifetimeContamination.SetTitle("Contamination as a Function of Lifetime")
fLifetimeContamination.GetXaxis().SetTitle("Lifetime (ms)")
fLifetimeContamination.GetYaxis().SetTitle("Oxygen Equivalent Contamination (ppt)")
fLifetimeContamination.GetYaxis().SetTitleOffset(1.5);
fLifetimeContamination.Draw("l")

leg = ROOT.TLegend(0.4,0.68,0.8,0.88)
leg.SetFillColor(0)
leg.SetLineColor(0)
leg.AddEntry(fLifetimeContamination,"[O_{2}] = 3 #times 10^{-13} sec./#tau","l")
leg.Draw("SAME")

c1.Print("lifetimeContamination.png")

