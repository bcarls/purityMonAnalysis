import psycopg2
import time
import datetime

def demo():
    #-- channel name
    name = 'uB_ArPurity_PM02_1/LIFETIME'

    #-- get start and stop of 24-hour interval on hour boundary
    now_hour = 3600*int(time.time()/3600)
    time1 = datetime.datetime.fromtimestamp(now_hour-86400)
    time2 = datetime.datetime.fromtimestamp(now_hour)

    #-- connect to db and make the query
    #   If password isn't given, psycopg2 will obtain it from
    #   $HOME/.pgpass if it exists.
    conn = psycopg2.connect(host="smc-priv", user="", password="",
                            database="slowmoncon_archive")    
    cur = conn.cursor()
    cur.execute("select smpl_time, float_val from sample " 
         "where channel_id = (select channel_id from channel where name = %s)" 
         "and smpl_time > %s and smpl_time < %s;",
                (name, time1, time2) )

    #-- print it out
    print '|_. ', ' |_. '.join( d[0] for d in cur.description), '| '
    for row in cur:
        print '| ', ' | '.join( str(v) for v in row ), ' |'
    print

if __name__ == "__main__":
    demo()

