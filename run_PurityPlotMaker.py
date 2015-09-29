

import sys
import ROOT

def main(argv):
    
    ROOT.gROOT.ProcessLine(".L PurityPlotMaker.cxx")

    inputFile = ''
    inputFileList = ''
    nevts = 0
    prm = 0
    # Parse arguments
    args = argv[1:]
    while len(args) > 0:
        if args[0] == '-s' or args[0] == '--source' :
            if len(args) > 1:
                inputFile = args[1]
                del args[0:2]
        elif args[0] == '-S' or args[0] == '--source-list' :
            if len(args) > 1:
                inputFileList = args[1]
                del args[0:2]
        elif args[0] == '-p' or args[0] == '--purity-monitor' :
            if len(args) > 1:
                prm = int(args[1])
                del args[0:2]
        elif args[0] == '-b' :
            del args[0:1]
    if inputFile == '' and inputFileList == '':
        print 'No input file(s) specified. Use "-s" or "--source" to specify one. Additionally, a file list can be supplied with "-S" or "--source-list".'
        return 0




    # Read in file(s) and create the TChain c
    t = ROOT.PurityPlotMaker()
    inputFiles = []
    if inputFile != '':
        inputFiles.append(inputFile)
    elif inputFileList != '':
        inputFiles = open(inputFileList).read().splitlines()
    for f in inputFiles:
        print 'Adding input file: '+f+'.'
        t.AddDataFile(f)

    t.SetPurityMonitor(prm)
    t.MakePlots()





if __name__ == '__main__':
    sys.exit(main(sys.argv))








