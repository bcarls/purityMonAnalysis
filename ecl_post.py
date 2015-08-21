from ECLAPI import ECLConnection, ECLEntry
from os import path, access, R_OK

#URL = "http://dbweb4.fnal.gov:8080/ECL/demo"
URL="http://dbweb4.fnal.gov:8080/ECL/uboone"
#user = ""
#password = ""



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
    e = ECLEntry(category='Cryogenics Operations', tags=[], formname='default', 
                text='<font face="arial"> <b> DAILY PLOT! </b> <BR> Plots of the contamination (Oxygen, Water, Nitrogen) level in the last 24 hours.<BR> For the latest plots check <a href="http://www-microboone.fnal.gov/at_work/gas_analyzers_plot.php">here</a>. <BR> Each filename indicates which contaminant is shown in the plot and which subsystem the gas analyzer device is connected to:  <BR> <i> AFL </i> is the Ar fill line; <BR> <i> PD </i> is the pump discharge; <BR> <i> HVC </i> is the high voltage cryostat; <BR> <i> BDLO </i> is the buffer dewar liquid outlet; <BR> <i> MS1(2) </i> is the molecular sieve 1(2); <BR> <i> CS1(2) </i> is the Oxygen filter 1(2);<BR> <i> UG </i> is the utility gas. <BR> Prefix <i> HR </i> stands for High Resolution (ppb level) device. </font>', preformatted=True)

#       Optional. Set the author username. The user must be registered CRL user 
    e.setAuthor('acciarri')

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
    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_H2O_AFL.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('ten',PATH, image=None)
    else:
        print "Either file is missing or is not readable"  

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_H2O_PD.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('eleven',PATH, image=None)
    else:
        print "Either file is missing or is not readable"  
 
    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_H2O_HVC.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twelve',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 
 
    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_H2O_BDLO.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('thirteen',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_H2O_MS2.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('fourteen',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_H2O_CS2.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('fifteen',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_H2O_MS1.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('sixteen',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_H2O_CS1.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('seventeen',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_H2O_UG.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('eighteen',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_O2_AFL.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('nineteen',PATH, image=None)
    else:
        print "Either file is missing or is not readable"  

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_O2_PD.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twenty',PATH, image=None)
    else:
        print "Either file is missing or is not readable"  
 
    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_O2_HVC.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twentyone',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 
 
    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_O2_BDLO.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twentytwo',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_O2_MS2.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twentythree',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_O2_CS2.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twentyfour',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_O2_MS1.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twentyfive',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_O2_CS1.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twentysix',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/HR_O2_UG.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twentyseven',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/N2_AFL.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twentyeight',PATH, image=None)
    else:
        print "Either file is missing or is not readable"  

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/N2_PD.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('twentynine',PATH, image=None)
    else:
        print "Either file is missing or is not readable"  
 
    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/N2_HVC.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('thirty',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 
 
    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/N2_BDLO.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('thirtyone',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/N2_MS2.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('thirtytwo',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/N2_CS2.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('thirtythree',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/N2_MS1.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('thirtyfour',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/N2_CS1.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('thirtyfive',PATH, image=None)
    else:
        print "Either file is missing or is not readable" 

    PATH='/afs/fnal.gov/files/expwww/microboone/html/at_work/gas_analyzers/ECL/GAS_Plots/N2_UG.png'
    if path.isfile(PATH) and access(PATH, R_OK):
        e.addImage('thirtysix',PATH, image=None)
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
