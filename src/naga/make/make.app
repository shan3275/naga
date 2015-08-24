include $(MKDIR)/make.var


CFLAGS  += -DMAKEAPP -g -Wall -I. -I$(SRCDIR)/../boots -I$(SRCDIR)/include
LDFLAGS += -L$(SRCDIR)/md5lib

all: $(PROG)

%.o: %.c
	$(CC) $(CFLAGS) $(ECFLAGS) -c $< -o $@


$(PROG): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS) $(ELDFLAGS) $(LDLIBS)

clean:
	rm -rf *.o
	rm -rf $(PROG)

