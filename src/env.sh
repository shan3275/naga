export RTE_SDK=`pwd`/libdpdk/dpdk
export RTE_TARGET=x86_64-native-linuxapp-gcc
export DESTDIR=`pwd`/libdpdk/dpdk
make -C libdpdk
make -C pcre-m
make -C http_parser
make -C libzmq
make -C libevent
