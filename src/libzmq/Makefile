.PHONY:all

all:zeromq-4.1.3 libzmq.a

libzmq.a:
	ln -s  zeromq-4.1.3/.libs/libzmq.a

zeromq-4.1.3:
	tar xvf zeromq-4.1.3.tar.gz>>/dev/null
	ln -s zeromq-4.1.3 zeromq
	cd zeromq && ./configure CFLAGS=-g --without-libsodium  >>/dev/null
	cd zeromq &&  make >>/dev/null


clean:
	rm -rf  zeromq-4.1.3
	rm -rf  libzmq.a
	rm -rf  zeromq
