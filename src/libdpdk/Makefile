.PHONY:all

all:dpdk-stable-18.11.2 

dpdk-stable-18.11.2:
	tar xvJf dpdk-18.11.2.tar.xz >>/dev/zero
	ln -s dpdk-stable-18.11.2 dpdk
	cd dpdk && make install T=$(RTE_TARGET)  

clean:
	rm -rf  dpdk-stable-18.11.2
	rm -rf  dpdk
