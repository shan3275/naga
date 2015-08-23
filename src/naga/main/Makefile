PROG    = naga

ECFLAGS = -g -O0  -I$(SRCDIR)/libzebra -I$(SRCDIR)/cmdline -I. #-Werror

ELDFLAGS = -L $(SRCDIR)/libzebra -L $(SRCDIR)/cmdline
LDLIBS  =  -lcmdline -lzebra -lcrypt -lreadline -lrt

include $(MKDIR)/make.app

