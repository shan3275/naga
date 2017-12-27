#!/usr/bin/python
# -*- coding: UTF-8 -*
#Filename:readline.py
import time
import os
import sys
import MySQLdb
import random

##查询数据库
##@key : 关键字
##@db_table :  database table
##return : 返回查询结果，单条语句
def query_db(key, value, db_table):
    conn=MySQLdb.connect(host='localhost',user='root',passwd='hai@yao',db='injection_test',port=3306, charset='utf8')
    cur=conn.cursor()
    sql = "select * from %s where %s='%s'" %(db_table, key, value)
    cur.execute(sql)
    info = cur.fetchone()
    cur.close()
    conn.close()
    return info

##插入数据库
##@entry : 表项
##@db_table :  database table
def insert_db(entry, db_table):
    conn=MySQLdb.connect(host='127.0.0.1',user='root',passwd='hai@yao',db='injection_test',port=3306,charset='utf8')
    cur=conn.cursor()
    sql = "INSERT INTO `%s` VALUES %s" %(db_table, entry)
    print sql
    cur.execute(sql)
    conn.commit()
    cur.close()
    conn.close()

##更新考勤数据到数据库中
##@file : 输入文件名
def update_traffic_data(day):
    for hour in range(0,24, 1):
	    dt = "%s %d:%d:%d" %(day, hour, random.randint(0, 30), random.randint(0, 40) )
	    #转换成时间数组
	    timeArray = time.strptime(dt, "%Y-%m-%d %H:%M:%S")
	    #print timeArray
	    #转换成时间戳
	    timestamp = time.mktime(timeArray)
            entry = "(null, '0', '0', '%d', '%d', '%d', '%d')" %(timestamp, random.randint(0, 1024000), random.randint(0, 1024000), random.randint(0, 1024000))
            print entry
            insert_db(entry, 'iface_flow_component')



if __name__ == "__main__":
    ##手动输入参数，格式：2016-08-30
    if sys.argv[1]:
        day = sys.argv[1]
    else:
        day = datetime.date.today() - datetime.timedelta(days=1)
    print day
    update_traffic_data(day)

