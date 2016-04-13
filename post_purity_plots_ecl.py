from ECLAPI import ECLConnection, ECLEntry
from os import path, access, R_OK
import commands

#URL = "http://dbweb4.fnal.gov:8080/ECL/demo"
URL="http://dbweb5.fnal.gov:8080/ECL/uboone"

user = commands.getoutput("aescrypt -d -p $(cat pass.aes) -o - ~/.pass.txt.aes | grep ECL | awk '{print $2}'")
password = commands.getoutput("aescrypt -d -p $(cat pass.aes) -o - ~/.pass.txt.aes | grep ECL | awk '{print $3}'")


if __name__ == '__main__':
    '''
    '''
    import getopt, sys
#    print "here1"   
    opts, args = getopt.getopt(sys.argv[1:], 'nU:')

    print_only = False

    for opt, val in opts:
        if opt=='-n':
            print_only = True
            
        if opt == '-U':
            URL = val

#################
#   Create test entry
    # e = ECLEntry(category='Cryogenics Operations', tags=[], formname='default', 
    #             text='<font face="arial"> <b> DAILY PLOT! </b> <BR> Plots of the contamination (Oxygen, Water, Nitrogen) level in the last 24 hours.<BR> For the latest plots check <a href="http://www-microboone.fnal.gov/at_work/gas_analyzers_plot.php">here</a>. <BR> Each filename indicates which contaminant is shown in the plot and which subsystem the gas analyzer device is connected to:  <BR> <i> AFL </i> is the Ar fill line; <BR> <i> PD </i> is the pump discharge; <BR> <i> HVC </i> is the high voltage cryostat; <BR> <i> BDLO </i> is the buffer dewar liquid outlet; <BR> <i> MS1(2) </i> is the molecular sieve 1(2); <BR> <i> CS1(2) </i> is the Oxygen filter 1(2);<BR> <i> UG </i> is the utility gas. <BR> Prefix <i> HR </i> stands for High Resolution (ppb level) device. </font>', preformatted=True)

    e = ECLEntry(category='Purity Monitor', tags=[], formname='default', 
                text='<font face="arial"> <b>Eight Hour Plots!</b> <BR> Plots from the long purity monitor for QA/QC, QA, QC, lifetime, and average lifetime. </font>', preformatted=True)
#       Optional. Set the author username. The user must be registered CRL user 
    e.setAuthor('bcarls')





#   Attach some file
#    e.addAttachment('attached-file', '/bin/zcat', 
#        data='Data may come here as an argument. The file will not be read in this case')

#   Attach some image
# Image data also can be passed as a parameter here using 'image' argument.
#    e.addImage('one','/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/O2_AFL.png', image=None)    
#    e.addImage('two','/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/O2_PD.png', image=None)
#    e.addImage('three','/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/O2_HVC.png', image=None)
#    e.addImage('four','/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/O2_BDLO.png', image=None)
#    e.addImage('five','/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/O2_MS2.png', image=None)
#    e.addImage('six','/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/O2_CS2.png', image=None)
#    e.addImage('seven','/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/O2_MS1.png', image=None)
#    e.addImage('eight','/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/O2_CS1.png', image=None)
#    e.addImage('nine','/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/O2_UG.png', image=None)


    PATH='/home/bcarls/purityMonAnalysis/QAQC_1.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('ten',PATH, image=None)
    else:
        print "Either file is missing or is not readable"  

    PATH='/home/bcarls/purityMonAnalysis/lifetime_1.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('eleven',PATH, image=None)
    else:
        print "Either file is missing or is not readable"  

    PATH='/home/bcarls/purityMonAnalysis/averageLifetime_1.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twelve',PATH, image=None)
    else:
        print "Either file is missing or is not readable"  

    PATH='/home/bcarls/purityMonAnalysis/QA_1.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('thirteen',PATH, image=None)
    else:
        print "Either file is missing or is not readable"  

    PATH='/home/bcarls/purityMonAnalysis/QC_1.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('fourteen',PATH, image=None)
    else:
        print "Either file is missing or is not readable"  




    if not print_only:
        # Define connection        print "here4"
#        print URL
#        print "here3"
        elconn = ECLConnection(URL, user, password)
#        print "here4",user,password
        #
        # The user should be a special user created with the "raw password" by administrator.
        # This user cannot login via GUI
        # The password should be kept in some file protected from other users, like postgres does with .pgpass file.
        # Postgres checks if .pgpass mode <= 0600 before uses the file.

        # Post test entry created above
        response = elconn.post(e)
#        print "here5"
        # Print what we have got back from the server
        print response
#        print "here6"
        # Close the connection
        elconn.close()
    else:
        # Just print prepared XML
        print e.xshow()
