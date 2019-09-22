.PHONY:all

all:hiredis-0.14.0 libhiredis.a

libhiredis.a:
	ln -s  hiredis-0.14.0/libhiredis.a

hiredis-0.14.0:
	tar xvf hiredis-0.14.0.tar.gz>>/dev/null
	ln -s hiredis-0.14.0 hiredis
	cd hiredis &&  make >>/dev/null


clean:
	rm -rf  hiredis-0.14.0
	rm -rf  libhiredis.a
	rm -rf  hiredis
