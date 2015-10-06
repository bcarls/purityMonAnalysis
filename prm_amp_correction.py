
import math

VA = [0.00509778,0.00510855,0.00511661,0.00514682,0.0052308]
VC = [0.00723007,0.00701563,0.00735314,0.00749103,0.00723971]

VAP = [0.00520882,0.00528955,0.00521568,0.00532116,0.00516007]
VCP = [0.00729419,0.00711567,0.00719871,0.00756494,0.00732977]

VAMean = 0
VCMean = 0
for V in VA:
    VAMean = VAMean + V
VAMean = VAMean/len(VA)

for V in VC:
    VCMean = VCMean + V
VCMean = VCMean/len(VC)

VASigma = 0
VCSigma = 0
for V in VA:
    VASigma += (VAMean - V)*(VAMean - V)
VASigma = math.sqrt(VASigma/(len(VA)*len(VA)))

for V in VC:
    VCSigma += (VCMean - V)*(VCMean - V)
VCSigma = math.sqrt(VCSigma/(len(VC)*len(VC)))


# sigma(V_A/A_C)^2 =   (1/V_C )^2 * sigma(V_A)^2 + (V_A/V_C^2)^2 * sigma(V_C)^2

VAVCSigma = math.sqrt( (1/VCMean)*(1/VCMean)*VASigma*VASigma + (VAMean/(VCMean*VCMean))*(VAMean/(VCMean*VCMean))*VCSigma*VCSigma)


VAPMean = 0
VCPMean = 0
for V in VAP:
    VAPMean = VAPMean + V
VAPMean = VAPMean/len(VAP)

for V in VCP:
    VCPMean = VCPMean + V
VCPMean = VCPMean/len(VAP)

VAPSigma = 0
VCPSigma = 0
for V in VAP:
    VAPSigma += (VAPMean - V)*(VAPMean - V)
VAPSigma = math.sqrt(VAPSigma/(len(VAP)*len(VAP)))

for V in VCP:
    VCPSigma += (VCPMean - V)*(VCPMean - V)
VCPSigma = math.sqrt(VCPSigma/(len(VCP)*len(VCP)))


VAPVCPSigma = math.sqrt( (1/VCPMean)*(1/VCPMean)*VAPSigma*VAPSigma + (VAPMean/(VCPMean*VCPMean))*(VAPMean/(VCPMean*VCPMean))*VCPSigma*VCPSigma)




print "VAMean: %f +/- %f" % (VAMean,VASigma)
print "VCMean: %f +/- %f" % (VCMean,VCSigma)
print "VAPMean: %f +/- %f" % (VAPMean,VAPSigma)
print "VCPMean: %f +/- %f" % (VCPMean,VCPSigma)

VAVC = VAMean/VCMean
VAPVCP = VAPMean/VCPMean

print "VAVC: %f +/- %f" % (VAVC,VAVCSigma)
print "VAPVCP: %f +/- %f" % (VAPVCP,VAPVCPSigma)

gagbsquared = (VAMean/VCMean)/(VAPMean/VCPMean)
gagbSquaredSigma = math.sqrt( (1/VAPVCP)*(1/VAPVCP)*VAVCSigma*VAVCSigma + (VAVC/(VAPVCP*VAPVCP))*(VAPMean/(VAPVCP*VAPVCP))*VCPSigma*VCPSigma)
gagb = math.sqrt((VAMean/VCMean)/(VAPMean/VCPMean))
gagbSigma = math.sqrt( 0.5*(1/math.sqrt(gagb))*(1/math.sqrt(gagb))*gagbSquaredSigma*gagbSquaredSigma)


print "g_alpha/g_beta: %f +/- %f" % (gagb,gagbSigma)


# Find the error on g_beta/g_alpha 

gbga = 1/gagb

gbgaSigma = math.sqrt(pow(1/gagb,4)*pow(gagbSigma,2))

print "g_beta/g_alpha: %f +/- %f" % (gbga,gbgaSigma)

