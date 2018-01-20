#!/bin/sh

CNT=0

while [ $CNT -lt 20000 ]
do
    tcpreplay -i p7p1 -L 1 111.pcap
    let CNT++
    echo $CNT
done
