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
import binascii
import gzip
import re
from cStringIO import StringIO



default_log_file = '/root/log/200OK.log'

def gzip_uncompress(c_data):
    buf = StringIO(c_data)
    f = gzip.GzipFile(mode='rb', fileobj=buf)
    try:
        r_data = f.read()
    finally:
        f.close()
    return r_data

def gzip_compress(raw_data):
    buf = StringIO()
    f = gzip.GzipFile(mode='wb', fileobj=buf)
    try:
        f.write(raw_data)
    finally:
        f.close()
    return buf.getvalue()

def chunk(data):
    unchunked=data
    pos = 0
    while pos <= len(data):
        chunkNumLen = data.find(b'\r\n', pos) - pos
        #  从第一个元素开始，发现第一个\r\n，计算length长度
        chunkLen = int(data[pos:pos + chunkNumLen], 16)
         # 把length的长度转换成int
        if chunkLen == 0:
            break
             # 如果长度为0，则说明到结尾
        chunk = data[pos + chunkNumLen + len('\r\n'):pos + chunkNumLen + len('\r\n') + chunkLen]
        unchunked += chunk
         # 将压缩数据拼接
        pos += chunkNumLen + len('\r\n') + chunkLen + len('\r\n')
         # 同时pos位置向后移动

    return unchunked

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
    conn=MySQLdb.connect(host='192.168.0.4',user='root',passwd='hai@yao',db='injection_test',port=3306,charset='utf8')
    cur=conn.cursor()
    sql = "INSERT INTO `%s` VALUES %s" %(db_table, entry)
    # print sql
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
    block_len = 1600
    buff_len  = 1568
    fp = open(real_log_file, 'r')
    block = fp.read(block_len)
    buff_str=''
    Session_200OK={}

    while len(block) == block_len:
        timestamp,int_sip,int_dip,sport,dport,seq,ack,buff_len,buff=struct.unpack("QIIHHIII1568s", block)
        time_local = time.localtime(timestamp)
        dt = time.strftime("%Y-%m-%d %H:%M:%S",time_local)
        #print "timestamp:%lu  %s" %(timestamp, dt)
        sip = socket.inet_ntoa(struct.pack('I',socket.htonl(int_sip))) 
        dip = socket.inet_ntoa(struct.pack('I',socket.htonl(int_dip))) 
        print "timestamp:%s sip:%s dip:%s sport:%u dport:%u seq:%x ack:%x buff_len:%u" %(dt,sip,dip,sport,dport,seq,ack,buff_len)

        #if timestamp >= start_timestamp and timestamp <= end_timestamp:
        #    sdictkey = str(int_sip) + str(int_dip) + str(dport) + str(ack)
        #    print seq
        #    if (buff.find('HTTP/1.1 200 OK') == 0):
        #        Session_200OK[sdictkey] = {
        #            'timestamp': timestamp,
        #            'content_length': 0,
        #            'len': 0,
        #            'header': buff[0:buff_len],
        #            'header_ok': 'no',
        #            'gzip': 0,
        #            'next_seq': seq,
        #        }
        #        if(buff.find('\r\n\r\n')>0):
        #            Session_200OK[sdictkey]['header'] = Session_200OK[sdictkey]['header'] + buff[0:(buff.find('\r\n\r\n'))]
        #            Session_200OK[sdictkey]['content_length'] = re.search(r"Content-Length: (.*)",Session_200OK[sdictkey]['header'],re.M | re.I).group(1)
        #    else:
        #        if(buff.find('\r\n\r\n')>0):
        #            Session_200OK[sdictkey]['header'] = Session_200OK[sdictkey]['header'] + buff[0:(buff.find('\r\n\r\n'))]
        #            Session_200OK[sdictkey]['content_length'] = re.search(r"Content-Length: (.*)",Session_200OK[sdictkey]['header'],re.M | re.I).group(1)
        #            Session_200OK[sdictkey]['gzip'] = re.search(r"Content-Encoding: (.*)",Session_200OK[sdictkey]['header'],re.M | re.I).group(1)
        #            buff=buff[0:buff_len]
        #            buff_len=buff_len-buff.find('\r\n\r\n')-4

        #            buff=buff[buff.find('\r\n\r\n')+4:]

        #        Session_200OK[sdictkey]['len']=Session_200OK[sdictkey]['len']+buff_len
        #        buff_str += buff[0:buff_len]
        #        print buff_len
        #        print binascii.hexlify(buff)

        #        print Session_200OK[sdictkey]


        #        if(int(Session_200OK[sdictkey]['len'])==int(Session_200OK[sdictkey]['content_length']) and Session_200OK[sdictkey]['gzip']!=0):
        #            # 更新content
        #            if(Session_200OK[sdictkey]['gzip']==0):
        #                Session_200OK[sdictkey]['content']=buff_str
        #            else:
        #                Session_200OK[sdictkey]['content']=gzip_uncompress(buff_str)

        #            # content 文本长度判断
        #            if(len(Session_200OK[sdictkey]['content'])<21800):
        #                # 插入数据库
        #                entry = "(null, '0', '0', '%s', '0', '%s', '%s', '%s', '%s', '%s')" % (timestamp, int_sip, int_dip, sport, dport,MySQLdb.escape_string(Session_200OK[sdictkey]['content']))
        #                # print entry
        #                insert_db(entry, 'iface_http_session')
        #            buff_str=''
        #            # print Session_200OK.keys()
        #            # del Session_200OK[sdictkey]
        #            # print Session_200OK.keys()

        block = fp.read(block_len)

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
    # print day
    # print start_time_str
    # print end_time_str
    # print log_file
    start_timestamp,end_timestamp,real_log_file = para_deal(day,start_time_str,end_time_str,log_file)
    bussi_process(start_timestamp,end_timestamp,real_log_file)
