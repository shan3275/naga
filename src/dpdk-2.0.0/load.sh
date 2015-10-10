sudo ifconfig eth3 down
sudo modprobe uio
sudo insmod ./build/kmod/igb_uio.ko
sudo ./tools/dpdk_nic_bind.py --bind=igb_uio eth3
sudo ifconfig eth4 up promisc 
sudo ifconfig eth5 up promisc 
