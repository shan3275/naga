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



default_log_file        = '/root/log/200OK.log'
default_output_log_file = '/root/log/tmp/200OK.log'

##二分查找寻找时间戳边界值
##@real_log_file:日志文件名称
##@m            :二分查找值
##@direction    :二分查找方向，0：向上，1：向下。
##ps：由于多条表项中的timestamp可能重复，故需要寻找到边界值。
def search2(real_log_file, m, direction):
    block_len = 1600
    file_size = os.path.getsize(real_log_file)
    #print "file_size:%u" %(file_size)
    block_num = (int)(file_size/block_len)
    #print "block num:%u" %(block_num)
    fp = open(real_log_file, 'r')
    block = fp.read(block_len)
    low = 0
    high = block_num -1
    while low<=high:  
        mid = (int)((low + high)/2)  
        fp.seek(mid*block_len,0)
        block = fp.read(block_len)
        timestamp,int_sip,int_dip,sport,dport,seq,ack,buff_len,buff=struct.unpack("QIIHHIII1568s", block)
        #time_local = time.localtime(timestamp)
        #dt = time.strftime("%Y-%m-%d %H:%M:%S",time_local)
        #sip = socket.inet_ntoa(struct.pack('I',socket.htonl(int_sip))) 
        #dip = socket.inet_ntoa(struct.pack('I',socket.htonl(int_dip))) 
        #print "timestamp:%s sip:%s dip:%s sport:%u dport:%u seq:%x ack:%x buff_len:%u" %(dt,sip,dip,sport,dport,seq,ack,buff_len)
        midval = timestamp
        if midval<m:
            low = mid + 1
        elif midval>m:
            high = mid-1
        else:
            #print(mid)
            #向上寻找边界值
            if direction == 0:
                mid = mid -1
                #print(mid)
                while midval == m:
                    fp.seek(mid*block_len,0)
                    block = fp.read(block_len)
                    if len(block) != 1600:
                        mid = mid -1
                        break
                    timestamp,int_sip,int_dip,sport,dport,seq,ack,buff_len,buff=struct.unpack("QIIHHIII1568s", block)
                    #time_local = time.localtime(timestamp)
                    #dt = time.strftime("%Y-%m-%d %H:%M:%S",time_local)
                    #sip = socket.inet_ntoa(struct.pack('I',socket.htonl(int_sip))) 
                    #dip = socket.inet_ntoa(struct.pack('I',socket.htonl(int_dip))) 
                    #print "timestamp:%s sip:%s dip:%s sport:%u dport:%u seq:%x ack:%x buff_len:%u" %(dt,sip,dip,sport,dport,seq,ack,buff_len)
                    midval = timestamp
                    mid = mid -1
                mid = mid + 2
            else:
                mid = mid +1
                #print(mid)
                while midval == m:
                    fp.seek(mid*block_len,0)
                    block = fp.read(block_len)
                    if len(block) != 1600:
                        mid = mid +1
                        break
                    timestamp,int_sip,int_dip,sport,dport,seq,ack,buff_len,buff=struct.unpack("QIIHHIII1568s", block)
                    #time_local = time.localtime(timestamp)
                    #dt = time.strftime("%Y-%m-%d %H:%M:%S",time_local)
                    #sip = socket.inet_ntoa(struct.pack('I',socket.htonl(int_sip))) 
                    #dip = socket.inet_ntoa(struct.pack('I',socket.htonl(int_dip))) 
                    #print "timestamp:%s sip:%s dip:%s sport:%u dport:%u seq:%x ack:%x buff_len:%u" %(dt,sip,dip,sport,dport,seq,ack,buff_len)
                    midval = timestamp
                    mid = mid +1
                mid = mid - 2
            #print(mid)
            break
    #print(mid)
    fp.close()
    return(mid)

##业务处理
##@day:指定某天
##@real_log_file:日志文件真实目录
##@output_log_file:输出日志文件路径
def bussi_process(day,real_log_file,output_log_file):
    for hour in range(0,24, 1):
        for mintue in range(0,60, 10):
            start_time_str = '%s %.2u:%.2u:00' %(day, hour, mintue)
            end_time_str   = '%s %.2u:%.2u:59' %(day, hour, mintue+9)
            print '%s  ---- %s' %(start_time_str, end_time_str)
            #转换成时间数组
            timeArray = time.strptime(start_time_str, "%Y-%m-%d %H:%M:%S")
            #转换成时间戳
            start_timestamp = time.mktime(timeArray)
            #转换成时间数组
            timeArray = time.strptime(end_time_str, "%Y-%m-%d %H:%M:%S")
            #转换成时间戳
            end_timestamp = time.mktime(timeArray)
            start_block_num = search2(real_log_file, start_timestamp, 0)
            end_block_num   = search2(real_log_file, end_timestamp,  1)
            print "start_block_num:%d, end_block_num:%d" %(start_block_num, end_block_num)
            if start_block_num < 0 or end_block_num < 0:
                print "error"
                continue 
            if start_block_num == 0 and end_block_num == 0:
                print "error"
                continue
            block_len = 1600
            time_local = time.localtime(start_timestamp)
            dt = time.strftime("%Y%m%d%H%M%S",time_local)
            write_log_file = "%s%s" %(output_log_file, dt)
            fp = open(real_log_file, 'r')
            begin = start_block_num
            end   = end_block_num
            fp.seek(begin*block_len,0)
            fw = open(write_log_file, 'w')
            while begin <= end:
                block = fp.read(block_len)
                fw.write(block)
                fw.flush()
                begin = begin + 1
            fp.close()
            fw.close()
            print "输出文件：%s" %(write_log_file)
    return 

##处理输入参数，业务处理使用
##@day  : 输入日期，格式2017-12-01，格式为字符串
##@start_time_str: 开始输入时间字符串，格式为20:34:23
##@end_time_str:   结束输入时间字符串，格式为23:32:21
##@log_file: 日志文件名
##返回值
##@star_timestamp:开始时间的时间戳，整形
##@end_timestamp:结束时间的时间戳，整形
##@real_log_file:日志文件真实目录
def para_deal(day,log_file):
    real_log_file = "%s%s%s%s" %(log_file, day.split("-")[0], day.split("-")[1], day.split("-")[2])
    print "real_log_file:%s" %(real_log_file)
    return real_log_file


if __name__ == "__main__":
    ##手动输入参数，格式：2016-08-30  /root/log/zebra.log /root/log/tmp/zebra.log
    if len(sys.argv) == 4:
        day = sys.argv[1]
        log_file = sys.argv[2]
        output_log_file = sys.argv[3]
    else:
        day = datetime.date.today()
        day = "%s" %(day)
        log_file = default_log_file
        output_log_file = default_output_log_file
    # print day
    # print start_time_str
    # print end_time_str
    # print log_file
    real_log_file = para_deal(day,log_file)
    bussi_process(day,real_log_file,output_log_file)
