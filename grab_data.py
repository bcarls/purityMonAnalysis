import psycopg2
import time
import datetime

def demo():
    #-- channel name
    name0 = 'uB_ArPurity_PM02_1/LIFETIME'
    name1 = 'uB_ArPurity_PM02_1/CATHTRUE'
    name2 = 'uB_ArPurity_PM02_1/ANODETRUE'
    name3 = 'uB_ArPurity_PM02_1/CATHFACTOR'
    name4 = 'uB_ArPurity_PM02_1/ANODEFACTOR'

    #-- get start and stop of 24-hour interval on hour boundary
    # now_hour = 3600*int(time.time()/3600)
    now_hour = time.time()
    print now_hour
    # time1 = datetime.datetime.fromtimestamp(now_hour-86400)
    time2 = datetime.datetime.fromtimestamp(now_hour)
    time1 = datetime.datetime.strptime("8 18 2015 15 28 11","%m %d %Y %H %M %S")

    #-- connect to db and make the query
    #   If password isn't given, psycopg2 will obtain it from
    #   $HOME/.pgpass if it exists.
    conn = psycopg2.connect(host="smc-priv", user="", password="",
                            database="slowmoncon_archive")    
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
                (name0, time1, time2) )
   
    lifetimes = []
    QC = []
    QA = []

    #-- print it out
    print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
    for row in cur:
        print '| ', ' | '.join( str(v) for v in row ), ' |'
        lifetimes.append(row[1])
    for val in lifetimes:
        print val


    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (name1, time1, time2) )

    #-- print it out
    print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
    for row in cur:
        print '| ', ' | '.join( str(v) for v in row ), ' |'
        QC.append(row[1])
    for val in QC:
        print val

    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (name2, time1, time2) )

    #-- print it out
    print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
    for row in cur:
        print '| ', ' | '.join( str(v) for v in row ), ' |'
        QA.append(row[1])
    for val in QA:
        print val

    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (name3, time1, time2) )

    #-- print it out
    print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
    for row in cur:
        print '| ', ' | '.join( str(v) for v in row ), ' |'
    print

    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (name4, time1, time2) )

    #-- print it out
    print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
    for row in cur:
        print '| ', ' | '.join( str(v) for v in row ), ' |'
    print

    for qc, qa in zip(QC,QA):
        print qa/qc




if __name__ == "__main__":
    demo()

