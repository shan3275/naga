sudo ifconfig eth3 down
sudo ifconfig eth2 down
sudo modprobe uio
sudo insmod ./build/kmod/igb_uio.ko
sudo ./tools/dpdk_nic_bind.py --bind=igb_uio eth2 eth3 
