sudo ifconfig eth1 down
sudo modprobe uio
sudo insmod ./build/kmod/igb_uio.ko
sudo ./tools/dpdk_nic_bind.py --bind=igb_uio eth1
