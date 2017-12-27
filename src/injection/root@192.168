#!/usr/bin/python
# -*- coding: UTF-8 -*
#Filename:anay_302.py
#本文件从指定文件读取内容，帅选出指定时间段的302报文，并将报文写入数据库
import time
import datetime
import os
import sys
import MySQLdb
import random
import socket
import struct

default_log_file = '/root/log/zebra.log'
##查询数据库
##@key : 关键字
##@db_table :  database table
##return : 返回查询结果，单条语句
def query_db(key, value, db_table):
    conn=MySQLdb.connect(host='127.0.0.1',user='root',passwd='hai@yao',db='injection_test',port=3306, charset='utf8')
    cur=conn.cursor()
    sql = "select * from %s where %s='%s'" %(db_table, key, value)
    cur.execute(sql)
    info = cur.fetchall()
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
    #
    cur.execute(sql)
    conn.commit()
    cur.close()
    conn.close()

##更新考勤数据到数据库中
##@file : 输入文件名
def update_traffic_data(day):
    info = query_db('iface_id', '0', 'iface_traffic_rate')
    for sql in info:
        dt =  "%s" %(sql[3])
        timeArray = time.strptime(dt, "%Y-%m-%d %H:%M:%S")
        timestamp = int(time.mktime(timeArray))
        entry = "(null, '0', '0', '%d', '%s')" %(timestamp ,sql[4])
        insert_db(entry, 'iface_traffic_rate1')

##业务处理
##@star_timestamp:开始时间的时间戳，整形
##@end_timestamp:结束时间的时间戳，整形
##@real_log_file:日志文件真实目录
def bussi_process(start_timestamp,end_timestamp,real_log_file):
    fp = open(real_log_file, 'r')
    for line in fp:
        array = line.split()
        if array[3] == '302':
            dt = "%s %s" %(array[0], array[1])
            timeArray = time.strptime(dt, "%Y-%m-%d %H:%M:%S")
            timestamp = int(time.mktime(timeArray))
            if timestamp >= start_timestamp and timestamp <= end_timestamp:
                # print array
                sip = socket.ntohl(struct.unpack("I",socket.inet_aton(str(array[4])))[0])
                dip = socket.ntohl(struct.unpack("I",socket.inet_aton(str(array[5])))[0])
                sport = array[6]
                dport = array[7]
                content = array[10]
                entry = "(null, '0', '0', '%s', '1', '%s', '%s', '%s', '%s', '%s')" %(timestamp,sip, dip, sport, dport, content)
                # print entry
                insert_db(entry, 'iface_http_session')
    fp.close()

##处理输入参数，业务处理使用
##@day  : 输入日期，格式2017-12-01，格式为字符串
##@start_time_str: 开始输入时间字符串，格式为20:34:23
##@end_time_str:   结束输入时间字符串，格式为23:32:21
##@log_file: 日志文件名
##返回值
##@star_timestamp:开始时间的时间戳，整形
##@end_timestamp:结束时间的时间戳，整形
##@real_log_file:日志文件真实目录
def para_deal(day,start_time_str,end_time_str,log_file):
    start_time_dt = "%s %s" %(day, start_time_str)
    start_timeArray = time.strptime(start_time_dt, "%Y-%m-%d %H:%M:%S")
    start_timestamp = int(time.mktime(start_timeArray))
    end_time_dt = "%s %s" %(day, end_time_str)
    end_timeArray = time.strptime(end_time_dt, "%Y-%m-%d %H:%M:%S")
    end_timestamp = int(time.mktime(end_timeArray))
    real_log_file = "%s%s%s%s" %(log_file, day.split("-")[0], day.split("-")[1], day.split("-")[2])
    print "star_timestamp:%d, end_timestamp:%d, real_log_file:%s" %(start_timestamp, end_timestamp, real_log_file)
    return (start_timestamp, end_timestamp, real_log_file)


if __name__ == "__main__":
    ##手动输入参数，格式：2016-08-30 09:34:23 15:38:00 /root/log/zebra.log
    if len(sys.argv) == 5:
        day = sys.argv[1]
        start_time_str = sys.argv[2]
        end_time_str = sys.argv[3]
        log_file = sys.argv[4]
    else:
        day = datetime.date.today()
        day = "%s" %(day)
        start_time = datetime.datetime.now() - datetime.timedelta(seconds=600)
        start_time_str = start_time.strftime('%H:%M:%S')
        end_time = datetime.datetime.now() - datetime.timedelta(seconds=0)
        end_time_str = end_time.strftime('%H:%M:%S')
        log_file = default_log_file
    print day
    print start_time_str
    print end_time_str
    print log_file
    start_timestamp,end_timestamp,real_log_file = para_deal(day,start_time_str,end_time_str,log_file)
    bussi_process(start_timestamp,end_timestamp,real_log_file)

    #update_traffic_data(day)

