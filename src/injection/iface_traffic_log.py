#/usr/local/bin/python
#coding=utf-8
import sys,re,time,os
import json
import getopt
import MySQLdb
import random

def argv_probe(argv):
   interface = ''
   direction = ''
   iface_id  = ''
   type_id   = ''
   try:
       opts, args = getopt.getopt(argv[1:],"hi:d:f:t:",["interface=","direction=","iface_id=","type_id="])
   except getopt.GetoptError:
      print ("%s -i <interface> -d <rx|tx> -f <num> -t <0|1>" %(argv[0]))
      print ("   -i --interface eth0|eth1|enp0....")
      print ("   -d --direction tx|rx")
      print ("   -f --iface_id number for interface,from 0 ")
      print ("   -t --type_id  number for interface type,0 for injection interface; 1 for GET input interface ")
      print ("   for example:")
      print ("   %s -i eth0 -d tx -f 0 -t 0" %(argv[0]))
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print ("%s -i <interface> -d <rx|tx> -f <num> -t <0|1>" %(argv[0]))
         print ("   -i --interface eth0|eth1|enp0....")
         print ("   -d --direction tx|rx")
         print ("   -f --iface_id number for interface,from 0 ")
         print ("   -t --type_id  number for interface type,0 for injection interface; 1 for GET input interface ")
         print ("   for example:")
         print ("   %s -i eth0 -d tx -f 0 -t 0" %(argv[0]))
         sys.exit()
      elif opt in ("-i", "--interface"):
         interface = arg
      elif opt in ("-d", "--direction"):
         direction = arg
      elif opt in ("-f", "--iface_id"):
         iface_id = arg
      elif opt in ("-t", "--type_id"):
         type_id = arg

   if ( interface == '' or direction == '' or iface_id == '' or type_id == ''):
      print ("%s -i <interface> -d <rx|tx> -f <num> -t <0|1>" %(argv[0]))
      print ("   -i --interface eth0|eth1|enp0....")
      print ("   -d --direction tx|rx")
      print ("   -f --iface_id number for interface,from 0 ")
      print ("   -t --type_id  number for interface type,0 for injection interface; 1 for GET input interface ")
      print ("   for example:")
      print ("   %s -i eth0 -d tx -f 0 -t 0" %(argv[0]))
      sys.exit(2)

   return (interface, direction, iface_id, type_id)   

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


netcard = '/proc/net/dev'
def get_net_data(interface, direction):
    nc = netcard or '/proc/net/dev'
    fd = open(nc, "r")
    netcardstatus = False
    for line in fd.readlines():
        if line.find(interface) >= 0 :
            netcardstatus = True
            field = line.split()
            if direction.lower() == 'rx':
               Bytes = field[0].split(":")[1]
               Bytes = Bytes or field[1]
            elif direction.lower() == 'tx':
               Bytes = field[8]
            else:
               fd.close()
               sys.exit()
    if not netcardstatus:
        fd.close()
        print("net interface name error!")
        sys.exit()

    fd.close()
    return float(Bytes)


##更新数据到数据库中
##@file : 输入文件名
def update_traffic_data(iface_id, type_id, rate):
    #entry = "(null, '%s', '%s', '%s:00', '%s')" %(iface_id, type_id, time.strftime('%Y-%m-%d %H:%M'), rate)
    entry = "(null, '%s', '%s', '%d', '%s')" %(iface_id, type_id, int(time.time()), rate)
    print entry
    insert_db(entry, 'iface_traffic_rate')

def net_loop(interface, direction, iface_id, type_id):
    Bytes =  get_net_data(interface,direction)
    time.sleep(10)
    new_Bytes = get_net_data(interface, direction)
    bit_data = (new_Bytes - Bytes)*8
    real_rate = int(bit_data/10);
    message = "流量信息 %s %s %s方向一分钟流量 iface_id:%s type_id:%s 检查实时平均速率: %s bps, 发大财！！！"%(time.strftime('%Y-%m-%d %H:%M:%S'),interface, direction,iface_id, type_id, real_rate)
    print  message
    update_traffic_data(iface_id, type_id, real_rate)


if __name__ == "__main__":
   interface,direction,iface_id,type_id = argv_probe(sys.argv)
   net_loop(interface,direction,iface_id,type_id)
