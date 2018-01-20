.PHONY:all


all:http-parser.2.7.1 libhttp_parser.a

libhttp_parser.a:
	ln -s  http-parser.2.7.1/libhttp_parser.a

http-parser.2.7.1:
	ln -s http-parser.2.7.1 http_parser
	cd http_parser 
	make
	make parsertrace
	make url_parser
	make package

clean:
	cd http_parser && make clean
	rm -rf  libhttp_parser.a

