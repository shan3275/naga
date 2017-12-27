#!/bin/sh
path=/root/work/injection
python $path/iface_traffic_log.py -i em1 -d rx -f 0 -t 0  &>> $path/log/iface_traffic_em1.log
