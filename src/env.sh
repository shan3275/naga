export RTE_SDK=`pwd`/libdpdk/dpdk
export RTE_TARGET=x86_64-native-linuxapp-gcc
export DESTDIR=`pwd`/libdpdk/dpdk
make -C libdpdk
make -C naga/pcre-m/
make -C naga/http_parser/
make -C naga/libzmq/
make -C naga/libevent/
