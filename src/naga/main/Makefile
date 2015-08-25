PROG    = naga

ECFLAGS = -g -O0  -I$(SRCDIR)/libzebra -I$(SRCDIR)/cmdline -I$(SRCDIR)/vsr -I. #-Werror

ELDFLAGS = -L $(SRCDIR)/libzebra -L $(SRCDIR)/cmdline -L$(SRCDIR)/vsr
LDLIBS  =  -lcmdline -lvsr -lzebra -lcrypt -lreadline -lrt

include $(MKDIR)/make.app

