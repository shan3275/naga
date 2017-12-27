#!/usr/bin/python
# -*- coding: UTF-8 -*

import time
import datetime
import os
import sys
import MySQLdb


dbname='injection_test'
##查询数据库
##@key : 关键字
##@db_table :  database table
##return : 返回查询结果，单条语句
def check_ports(db_table):
    conn=MySQLdb.connect(host='127.0.0.1',user='root',passwd='hai@yao',db='control',port=3306, charset='utf8')
    cur=conn.cursor()
    sql = "select * from %s" %(db_table)
    cur.execute(sql)
    info = cur.fetchall()
    cur.close()
    conn.close()
    return info

def check_hit(ports,keyword):
    conn=MySQLdb.connect(host=ports[2],user=ports[4],passwd=ports[5],db=ports[3],port=int(ports[6]),charset=ports[7])
    cur=conn.cursor()
    sql = "select * from keyword_filter_table where id='%s'" %(keyword)
    cur.execute(sql)
    info = cur.fetchone()
    cur.close()
    conn.close()
    return info

##插入数据库
##@entry : 表项
##@db_table :  database table
def insert_db(ports,entry, db_table):
    conn=MySQLdb.connect(host=ports[2],user=ports[4],passwd=ports[5],db=ports[3],port=int(ports[6]),charset=ports[7])
    cur=conn.cursor()
    sql = "INSERT INTO `%s` VALUES %s" %(db_table, entry)
    cur.execute(sql)
    conn.commit()
    cur.close()
    conn.close()

##更新数据到数据库中
##@file : 输入文件名
def update_data(ports,id,keyword):
    conn=MySQLdb.connect(host=ports[2],user=ports[4],passwd=ports[5],db=ports[3],port=int(ports[6]),charset=ports[7])
    cur=conn.cursor()
    sql = "UPDATE keyword_filter_table SET keyword = '%s' WHERE id = %s" %(keyword,id)
    cur.execute(sql)
    conn.commit()
    cur.close()
    conn.close()

def update_count(id,count):
    conn=MySQLdb.connect(host='127.0.0.1',user='root',passwd='hai@yao',db='control',port=3306, charset='utf8')
    cur=conn.cursor()
    sql = "UPDATE con_admin_keywords SET count = '%s' WHERE id = %s" %(count,id)
    cur.execute(sql)
    conn.commit()
    cur.close()
    conn.close()

##删除数据
##@file : 输入文件名
def delete_db(ports,id):
    conn=MySQLdb.connect(host=ports[2],user=ports[4],passwd=ports[5],db=ports[3],port=int(ports[6]),charset=ports[7])
    cur=conn.cursor()
    sql = "DELETE FROM keyword_filter_table WHERE id = '%d'" % (id)
    cur.execute(sql)
    conn.commit()
    cur.close()
    conn.close()

def insert_count(ports_list,count_list):
    count={}
    for x in ports_list:
        for y in count_list:
            data=check_hit(x,y[0])
            if(data!=None):
                if(data[3]!=y[5]):
                    update_data(x,data[0],y[5])
                if(y[6]==0):
                    delete_db(x,data[0])
                else:
                    if(data[0] in count):
                        count[data[0]]+=data[4]
                    else:
                        count[data[0]]=data[4]
            elif(y[6]==1):
                entry = "('%d','%d','%d','%s',0,0)" % (y[0],int(time.time()), y[3],y[5])
                insert_db(x, entry, 'keyword_filter_table')
    for x in count:
        update_count(x,count[x])




ports_list=check_ports('con_admin_ports')
count_list=check_ports('con_admin_keywords')
insert_count(ports_list,count_list)