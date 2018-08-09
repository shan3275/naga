.PHONY:all

all:libevent-2.1.8-stable libevent.a

libevent.a:
	ln -s  libevent-2.1.8-stable/.libs/libevent.a

libevent-2.1.8-stable:
	tar xvf libevent-2.1.8.tar.gz>>/dev/null
	ln -s libevent-2.1.8-stable libevent
	cd libevent && ./configure CFLAGS=-g   >>/dev/null
	cd libevent &&  make >>/dev/null


clean:
	rm -rf  libevent-2.1.8-stable
	rm -rf  libevent.a
	rm -rf  libevent
