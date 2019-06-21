1. 系统要求：
CentOS 6.x 64

2. 程序配置更改
编辑zebra.conf ，需要修改的地方如下：
adp dmac set bc:9c:31:ba:4d:e6
adp smac set 3c:a8:2a:0e:99:54
rpush sip enable
rpush sip set 192.168.8.120
rpush add 211.149.173.172 15583
interface work-thread add 8
interface bussiness add eth2
interface bussiness add eth3
interface injection add eth0
说明：
adp dmac set 配置网关mac地址；
ddp smac set 配置回注口mac地址；
rpush sip enable 配置回注报文是否使用配置的源IP，enable使用配置源ip，disable不使用，使用报文源IP
rpush sip set 192.168.8.120 配置回注报文的源IP
rpush add 211.149.173.172 15583 配置回注报文的目的ip和端口
interface work-thread add 添加业务处理线程个数，也即业务处理core个数；
interface bussiness add 添加接收报文网口，一条命令添加一个接口；
interface injection add 添加回注口网口，只能添加一个回注口。

3.启动.
在当前目录下面执行。
./naga &

4.定时任务
为了每日清内部统计，在每日零点定时重启naga。重启需要修改如下：
4.1 naga_restart.sh 脚本中，将
cd /home/naga/bin/
修改为当前路径。
4.2 在/etc/crontab中添加定时任务,每日定时重启，定时任务：
0    0 * * * root /home/naga/bin/naga_restart.sh
路径修改为当前路径即可。

5.查看业务是否正常。
通过命令：telnet 127.0.0.1 2608
密码：rose
进入之后输入:show stat
统计有增加，代表发送成功。重点关注统计如下：
ACL_UDPPUSH_PAYLOAD_GEN                 : 82736
ACL_UDPPUSH_RAW_UDP_GEN                 : 82736
ACL_UDPPUSH_TX_SUCCESS                  : 82736
这几个统计代表发送的udp报文的统计。

