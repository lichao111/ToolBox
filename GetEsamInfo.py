import redis
import time
import csv

def GetKeyHasValue(value):
    pool=redis.ConnectionPool(host='20.76.189.169',port=19000,db=0,password='GScjyw@8954');
    Redis = redis.Redis(connection_pool=pool)
    KeyName = Redis.zrange('OnLineTerminal',0,-1,desc=False,withscores=False)
    #file = open("esamStatistic.csv","w")
    #file.write("terminal_addr\tsecurity_state\tkey_state"+"\n")
    with open("esam.csv","w") as datacsv:
        csvwriter = csv.writer(datacsv,dialect = ("excel"))
        csvwriter.writerow(["terminal_addr","security_state","key_state"])
        for i in KeyName:
            security_state =  Redis.hget("APPLINK:"+i,"security_state")
            key_state =  Redis.hget("APPLINK:"+i,"key_state")
            if(security_state==None): 
                security_state = 0
            if(key_state==None): 
                key_state = 0
            csvwriter.writerow([i,security_state,key_state])
            #ddfile.write(i+"\t"+security_state+"\t"+key_state+"\n")

def GetAllKeys():
    return KeyaName

if __name__=='__main__':
    start = time.time()
    print("start time:",start)
    GetKeyHasValue('0')
    print("used time: ",time.time()-start)
