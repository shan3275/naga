#/usr/bin/python
#coding=utf-8

import re
import os
import sys
import fileinput
import MySQLdb
import time
from datetime import datetime

logdata=[]

date = datetime.now().strftime("%Y%m%d")
log_file = '/root/log/stat.log'+date
file_data=fileinput.input(log_file)
for line in file_data:
    logdata.append(line.strip('\n'))

num_302 = int(logdata[len(logdata)-3].split(':')[1].strip()) - int(logdata[len(logdata)-3-60*19].split(':')[1].strip())
num_200 = int(logdata[len(logdata)-5].split(':')[1].strip()) - int(logdata[len(logdata)-5-60*19].split(':')[1].strip())
num_other = int(logdata[len(logdata)-18].split(':')[1].strip()) - int(logdata[len(logdata)-18-60*19].split(':')[1].strip()) - num_302 - num_200

def insert_db(entry, db_table):
    conn=MySQLdb.connect(host='127.0.0.1',user='root',passwd='hai@yao',db='injection_test',port=3306,charset='utf8')
    cur=conn.cursor()
    sql = "INSERT INTO `%s` VALUES %s" %(db_table, entry)
    print sql
    cur.execute(sql)
    conn.commit()
    cur.close()
    conn.close()


now = int(time.time())

entry = "(null, '0', '0', '%d', '%d', '%d', '%d')" %(now, num_302, num_200, num_other)

insert_db(entry,'iface_flow_component')
