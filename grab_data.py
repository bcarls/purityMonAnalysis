import psycopg2
import time
import datetime
import commands

def demo():

    #-- Grab user and password
    userGrab = commands.getoutput("aescrypt -d -p $(cat pass.aes) -o - ~/.pass.txt.aes | grep smc-priv | awk '{print $2}'")
    passwordGrab  = commands.getoutput("aescrypt -d -p $(cat pass.aes) -o - ~/.pass.txt.aes | grep smc-priv | awk '{print $3}'")
    
    #-- channel name
    nameLifetime  = 'uB_ArPurity_PM02_1/LIFETIME'
    nameQC        = 'uB_ArPurity_PM02_1/CATHTRUE'
    nameQA        = 'uB_ArPurity_PM02_1/ANODETRUE'
    nameCatBase   = 'uB_ArPurity_PM02_1/CATHBASE'
    nameAnoBase   = 'uB_ArPurity_PM02_1/ANODEBASE'
    nameCathF     = 'uB_ArPurity_PM02_1/CATHFACTOR'
    nameAnoF      = 'uB_ArPurity_PM02_1/ANODEFACTOR'

    #-- get start and stop of 24-hour interval on hour boundary
    # now_hour = 3600*int(time.time()/3600)
    now_hour = time.time()
    # print now_hour
    # time1 = datetime.datetime.fromtimestamp(now_hour-86400)
    time1 = datetime.datetime.fromtimestamp(now_hour-7*24*60*60)
    time2 = datetime.datetime.fromtimestamp(now_hour)
    # time1 = datetime.datetime.strptime("11 18 2015 15 28 11","%m %d %Y %H %M %S")

    #-- connect to db and make the query
    #   If password isn't given, psycopg2 will obtain it from
    #   $HOME/.pgpass if it exists.
    # conn = psycopg2.connect(host="smc-priv", user="", password="",
    #                         database="slowmoncon_archive")    
    conn = psycopg2.connect(host="smc-priv", user=userGrab, password=passwordGrab, database="slowmoncon_archive")    
    cur = conn.cursor()
    # cur.execute("select smpl_time, float_val, channel_id from sample "
    #      "where channel_id = (select channel_id from channel where name = %s)" 
    #      "and smpl_time > %s and smpl_time < %s "
    #      "union select smpl_time, float_val, channel_id from sample " 
    #      "where channel_id = (select channel_id from channel where name = %s)" 
    #      "and smpl_time > %s and smpl_time < %s;",
    #             (name0, time1, time2 , name1, time1, time2) )


    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (nameLifetime, time1, time2) )
   
    Lifetime = []
    TimeStamp = []
    QC = []
    QA = []
    CatBase = []
    AnoBase = []
    CathF = []
    AnoF = []
    #-- print it out
    # print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
    for row in cur:
        print '| ', ' | '.join( str(v) for v in row ), ' |'
        Lifetime.append(row[1])
        TimeStamp.append(row[0])
    # for val in lifetime:
        # print val
    # for val in TimeStamp:
    #     print val




    ###############
    # Get QC
    ###############

    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (nameQC, time1, time2) )

    # for row in cur:
    #     print row[0], row[1]

    for timestamp in TimeStamp:
        foundVariable = False
        foundValue = 0
        # print "timestamp"
        # print timestamp
        # print "rows in cur"
        cur.scroll(0,mode="absolute")
        for row in cur:
            # print row[0]
            elapsed = timestamp - row[0]
            # print elapsed.total_seconds()
            if abs(elapsed.total_seconds()) < 1:
                foundVariable = True
                foundValue = row[1]
                break
        if foundVariable == True:
            QC.append(foundValue)
            # print "Found variable"
        elif foundVariable == False:
            QC.append(foundValue)
            # print "Found no variable"
    # for timestamp,lifetime,val in zip(TimeStamp,Lifetime,QC):
    #     print timestamp,lifetime,val

    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (nameQC, time1, time2) )

    #-- print it out
    # print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
    for row in cur:
        # print '| ', ' | '.join( str(v) for v in row ), ' |'
        QC.append(row[1])
    # for val in QC:
    #     print val









    ###############
    # Get QA
    ###############

    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (nameQA, time1, time2) )

    # for row in cur:
    #     print row[0], row[1]

    for timestamp in TimeStamp:
        foundVariable = False
        foundValue = 0
        # print "timestamp"
        # print timestamp
        # print "rows in cur"
        cur.scroll(0,mode="absolute")
        for row in cur:
            # print row[0]
            elapsed = timestamp - row[0]
            # print elapsed.total_seconds()
            if abs(elapsed.total_seconds()) < 1:
                foundVariable = True
                foundValue = row[1]
                break
        if foundVariable == True:
            QA.append(foundValue)
            # print "Found variable"
        elif foundVariable == False:
            QA.append(foundValue)
            # print "Found no variable"
    # for timestamp,lifetime,val in zip(TimeStamp,Lifetime,QA):
    #     print timestamp,lifetime,val

    # cur.execute("select smpl_time, float_val from sample " 
    #      "where channel_id = (select channel_id from channel where name = %s)" 
    #      "and smpl_time > %s and smpl_time < %s;",
    #             (nameQA, time1, time2) )

    # #-- print it out
    # # print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
    # for row in cur:
    #     # print '| ', ' | '.join( str(v) for v in row ), ' |'
    #     QA.append(row[1])
    # # for val in QA:
    # #     print val





    ###############
    # Get CatBase
    ###############

    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (nameCatBase, time1, time2) )

    # for row in cur:
    #     print row[0], row[1]

    for timestamp in TimeStamp:
        foundVariable = False
        foundValue = 0
        # print "timestamp"
        # print timestamp
        # print "rows in cur"
        cur.scroll(0,mode="absolute")
        for row in cur:
            # print row[0]
            elapsed = timestamp - row[0]
            # print elapsed.total_seconds()
            if abs(elapsed.total_seconds()) < 1:
                foundVariable = True
                foundValue = row[1]
                break
        if foundVariable == True:
            CatBase.append(foundValue)
            # print "Found variable"
        elif foundVariable == False:
            CatBase.append(foundValue)
            # print "Found no variable"
    # for timestamp,lifetime,val in zip(TimeStamp,Lifetime,CatBase):
    #     print timestamp,lifetime,val

    # cur.execute("select smpl_time, float_val from sample " 
    #      "where channel_id = (select channel_id from channel where name = %s)" 
    #      "and smpl_time > %s and smpl_time < %s;",
    #             (nameCatBase, time1, time2) )

    # #-- print it out
    # # print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
    # for row in cur:
    #     # print '| ', ' | '.join( str(v) for v in row ), ' |'
    #     CatBase.append(row[1])
    # # print








    ###############
    # Get AnoBase
    ###############

    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (nameAnoBase, time1, time2) )

    # for row in cur:
    #     print row[0], row[1]

    for timestamp in TimeStamp:
        foundVariable = False
        foundValue = 0
        # print "timestamp"
        # print timestamp
        # print "rows in cur"
        cur.scroll(0,mode="absolute")
        for row in cur:
            # print row[0]
            elapsed = timestamp - row[0]
            # print elapsed.total_seconds()
            if abs(elapsed.total_seconds()) < 1:
                foundVariable = True
                foundValue = row[1]
                break
        if foundVariable == True:
            AnoBase.append(foundValue)
            # print "Found variable"
        elif foundVariable == False:
            AnoBase.append(foundValue)
            # print "Found no variable"
    # for timestamp,lifetime,val in zip(TimeStamp,Lifetime,AnoBase):
    #     print timestamp,lifetime,val

    # cur.execute("select smpl_time, float_val from sample " 
    #      "where channel_id = (select channel_id from channel where name = %s)" 
    #      "and smpl_time > %s and smpl_time < %s;",
    #             (nameAnoBase, time1, time2) )

    # #-- print it out
    # # print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
    # for row in cur:
    #     # print '| ', ' | '.join( str(v) for v in row ), ' |'
    #     AnoBase.append(row[1])
    # # print




    ###############
    # Get CathF
    ###############

    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (nameCathF, time1, time2) )

    # for row in cur:
    #     print row[0], row[1]

    for timestamp in TimeStamp:
        foundVariable = False
        foundValue = 0
        # print "timestamp"
        # print timestamp
        # print "rows in cur"
        cur.scroll(0,mode="absolute")
        for row in cur:
            # print row[0]
            elapsed = timestamp - row[0]
            # print elapsed.total_seconds()
            if abs(elapsed.total_seconds()) < 1:
                foundVariable = True
                foundValue = row[1]
                break
        if foundVariable == True:
            CathF.append(foundValue)
            # print "Found variable"
        elif foundVariable == False:
            CathF.append(foundValue)
            # print "Found no variable"
    # for timestamp,lifetime,val in zip(TimeStamp,Lifetime,CathF):
    #     print timestamp,lifetime,val




    ###############
    # Get AnoF
    ###############

    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (nameAnoF, time1, time2) )

    # for row in cur:
    #     print row[0], row[1]

    for timestamp in TimeStamp:
        foundVariable = False
        foundValue = 0
        # print "timestamp"
        # print timestamp
        # print "rows in cur"
        cur.scroll(0,mode="absolute")
        for row in cur:
            # print row[0]
            elapsed = timestamp - row[0]
            # print elapsed.total_seconds()
            if abs(elapsed.total_seconds()) < 1:
                foundVariable = True
                foundValue = row[1]
                break
        if foundVariable == True:
            AnoF.append(foundValue)
            # print "Found variable"
        elif foundVariable == False:
            AnoF.append(foundValue)
            # print "Found no variable"
    # for timestamp,lifetime,val in zip(TimeStamp,Lifetime,AnoF):
    #     print timestamp,lifetime,val

#     cur.execute("select smpl_time, float_val from sample " 
#          "where channel_id = (select channel_id from channel where name = %s)" 
#          "and smpl_time > %s and smpl_time < %s;",
#                 (nameAnoF, time1, time2) )

#     #-- print it out
#     # print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
#     for row in cur:
#         # print '| ', ' | '.join( str(v) for v in row ), ' |'
#         AnoF.append(row[1])
#     # print


    
    lifetime_file = open("PrM_Logs/lifetimes_smc_01.txt","w")
    for timestamp,lifetime,qc,qa,catbase,anobase,cathf,anof in zip(TimeStamp,Lifetime,QC,QA,CatBase,AnoBase,CathF,AnoF):
        if lifetime is not None:
            print timestamp.month,timestamp.day,timestamp.year,timestamp.hour,timestamp.minute,timestamp.second,lifetime,qc,qa,catbase,anobase,cathf,anof

            lifetime_file.write("%s %s %s %s %s %s %s %s %s %s %s %s %s \n" % (timestamp.month,timestamp.day,timestamp.year,timestamp.hour,timestamp.minute,timestamp.second,lifetime,qc,qa,catbase,anobase,cathf,anof))
    lifetime_file.close()

guess = "a"
correct = "a"
if guess == correct:
    print "That's it!\n"

if __name__ == "__main__":
    demo()

