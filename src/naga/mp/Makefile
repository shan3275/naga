PROG    = naga

ECFLAGS = -g -mssse3  -I$(SRCDIR)/libzebra -I$(SRCDIR)/cmdline -I$(SRCDIR)/vsr -I$(SRCDIR)/dpdk/x86_64-native-linuxapp-gcc/include  #-Werror

ELDFLAGS = -L $(SRCDIR)/libzebra -L $(SRCDIR)/cmdline -L$(SRCDIR)/vsr -L$(SRCDIR)/dpdk/x86_64-native-linuxapp-gcc/lib
LDLIBS  =  -lintel_dpdk -ldl -lcmdline -lvsr -lzebra -lcrypt -lreadline -lrt 
include $(MKDIR)/make.app

