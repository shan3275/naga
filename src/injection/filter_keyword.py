#!/usr/bin/python
# -*- coding: UTF-8 -*
#Filename:filter_keyword.py
#本文件从指定文件读取内容，帅选出指定时间段的302报文，并将报文写入数据库
import time
import datetime
import os
import sys
import MySQLdb
import MySQLdb.cursors
import random
import socket
import struct
import binascii
import gzip
import re
from cStringIO import StringIO

##查询数据库
##@key : 关键字
##@value: 值
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

##查询数据库
##@db_table :  database table
##return : 返回查询结果，单条语句
def query_db_all(db_table):
    conn=MySQLdb.connect(host='127.0.0.1',user='root',passwd='hai@yao',db='injection_test',port=3306, charset='utf8')
    cur=conn.cursor()
    sql = "select * from %s" %(db_table)
    cur.execute(sql)
    info = cur.fetchall()
    cur.close()
    conn.close()
    return info

##插入数据库
##@entry : 表项
##@db_table :  database table
def insert_db(entry, db_table):
    conn=MySQLdb.connect(host='192.168.0.4',user='root',passwd='hai@yao',db='injection_test',port=3306,charset='utf8')
    cur=conn.cursor()
    sql = "INSERT INTO `%s` VALUES %s" %(db_table, entry)
    # print sql
    cur.execute(sql)
    conn.commit()
    cur.close()
    conn.close()

##操作表项
##@sql : sql命令
def do_db(sql):
    conn=MySQLdb.connect(host='192.168.0.4',user='root',passwd='hai@yao',db='injection_test',port=3306,charset='utf8')
    cur=conn.cursor()
    cur.execute(sql)
    conn.commit()
    cur.close()
    conn.close()


##业务处理
##@real_log_file:日志文件真实目录
def bussi_process():
    filter_table = query_db_all('keyword_filter_table')
    for sql in filter_table:
        sql_id = sql[0]
        sql_timestamp = sql[1]
        sql_session_type_id = sql[2]
        sql_keyword = sql[3]
        sql_hitnum  = sql[4]
        sql_last_id = sql[5]
        print "%d %u %d %s %u %u" %(sql_id, sql_timestamp, sql_session_type_id, sql_keyword, sql_hitnum, sql_last_id)
        if sql_session_type_id<2:
            command = "SELECT id,timestamp,session_type_id,content FROM iface_http_session where id>%u and \
timestamp>=%u and session_type_id=%u and  content like '%%%s%%'" %(sql_last_id, sql_timestamp,sql_session_type_id, sql_keyword)
        else:
            command = "SELECT id,timestamp,session_type_id,content FROM iface_http_session where id>%u and \
timestamp>=%u and  content like '%%%s%%'" %(sql_last_id, sql_timestamp, sql_keyword)
        print command
        conn = MySQLdb.connect(host='127.0.0.1', user='root', passwd='hai@yao', db='injection_test', port=3306,
                                       charset='utf8', cursorclass = MySQLdb.cursors.SSCursor)
        cur = conn.cursor()
        cur.execute(command);
        row = cur.fetchone()
        hit_num = sql_hitnum
        row_id = sql_last_id
        while row is not None:
            ###计数
            hit_num = hit_num + 1
            ###命中将id插入hit_keyword_table表项中
            #print row
            row_id = row[0]
            entry = "(null, '%d', '%d')" %(sql_id ,row_id)
            #print entry
            insert_db(entry, 'hit_keyword_table')
            row = cur.fetchone()
        cur.close()
        conn.close()
        ##更新计数到数据中
        command = 'update keyword_filter_table t set t.hit_num=%u,t.last_id=%u where t.id=%u;' %(hit_num,row_id,sql_id)
        print command
        do_db(command)

if __name__ == "__main__":
    bussi_process()
