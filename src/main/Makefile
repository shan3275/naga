# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) 2010-2014 Intel Corporation

# binary name
APP = naga

# all source are stored in SRCS-y
SRCS-y := naga_acl.c naga_fct.c main_data.c main_init.c main_l3fwd.c l3fwd_lpm.c core_cmd.c main.c


ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif

# Default target, can be overridden by command line or environment
RTE_TARGET ?= x86_64-native-linuxapp-gcc

include $(RTE_SDK)/mk/rte.vars.mk

CFLAGS += -O3
CFLAGS += $(WERROR_FLAGS)

CFLAGS  += -I ../cmdline  -I ../libzebra  -I ../boots -I ../include  -I ../pid -I ../debug -I ../pub -I ../libzmq/zeromq/include -I ../upush
CFLAGS  += -I ../dmr -I ../acr -I ../adp -I ../netseg -I ../dnet -I ../urlr -I ../url -I ../hijack  -I ../rte -I ../pcre-m/pcre -I ../mask  -I ../template  -I ../itf
CFLAGS  += -I ../ua
LDFLAGS += -lstdc++
LDLIBS  +=  ../cmdline/libcmdline.a  ../libzebra/libzebra.a ../url/liburl.a
LDLIBS  +=  ../pid/libpid.a ../http_parser/libhttp_parser.a ../itf/libitf.a   ../dmr/libdmr.a ../acr/libacr.a ../boots/libboots.a ../pub/libpub.a ../urlr/liburlr.a ../mask/libmask.a
LDLIBS  +=   ../adp/libadp.a ../template/libtemp.a ../debug/libdebug.a  -lpthread ../itf/libitf.a ../netseg/libnetseg.a ../dnet/libdnetseg.a ../url/liburl.a ../pcre-m/libpcre.a ../hijack/libhijack.a  ../upush/libupush.a ../libzmq/libzmq.a
LDLIBS  +=   -lcrypt -lreadline -lrt -lpcap -lpthread -lz ../ua/libua.a ../libevent/libevent.a

include $(RTE_SDK)/mk/rte.app.mk
