#!/bin/bash

cd /home/bcarls/purityMonAnalysis/

# Setup PATH, ROOT, and psycopg2
export PATH=~/bin/:$PATH
source ~ubooneshift/setup_SMC_EPICS.sh
setup root v5_34_05 -q e2:prof
setup psycopg2 v2_5_4

# Get latest data from EPICS, every data gets its own file

# That doesn't work yet, use a Dropbox hack
dropbox_uploader.sh -s download /PrM_Logs/

# Need to create a list of data files
rm list_of_datafiles.txt
# for l in PrM_Logs_Stat_Uncert/lifetimes_0"$1"*.txt
for l in PrM_Logs/lifetimes_0"$1"*.txt
do
    echo "Adding $l to the list."
    echo $l >> list_of_datafiles.txt
done

# Run the python script to plot lifetime
python run_PurityPlotMaker.py -S list_of_datafiles.txt -p $1 -b 

# Post the plots to the ECL
if [[ $HOSTNAME == *"ubdaq"* ]]; then
    echo "Submitting to the ECL."
    python post_purity_plots_ecl.py 
fi
