echo 1024 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages
echo 1024 > /sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages
mkdir -p /mnt/huge
mount -t hugetlbfs nodev /mnt/huge
ifconfig p4p1 down
modprobe uio
insmod igb_uio.ko
insmod rte_kni.ko
./dpdk-devbind.py --bind=igb_uio p4p1
