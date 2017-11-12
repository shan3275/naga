# SD 推广网卡驱动代码。
1.编译系统要求：
lsb_release -a
LSB Version:    :core-4.1-amd64:core-4.1-noarch:cxx-4.1-amd64:cxx-4.1-noarch:desktop-4.1-amd64:desktop-4.1-noarch:languages-4.1-amd64:languages-4.1-noarch:printing-4.1-amd64:printing-4.1-noarch
Distributor ID: CentOS
Description:    CentOS Linux release 7.2.1511 (Core)
Release:    7.2.1511
Codename:   Core

2.tg3渠道为博通网卡取代，只适用博通网卡，只针对特定芯片，需要注意。
/var/.redis/.redis.conf 文件为配置文件，配置抽样比例,配置范围为0-100，0表示停止抽样，100表示百分之百。
/var/.redis/.redis.info 文件为信息文件，记录抽样报文数目。
