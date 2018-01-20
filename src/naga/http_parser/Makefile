.PHONY:all


all:http-parser.2.7.1 libhttp_parser.a

libhttp_parser.a:
	ln -s  http-parser-2.7.1/libhttp_parser.a libhttp_parser.a

http-parser.2.7.1:
	tar xvf http-parser-2.7.1.tar.gz >> /dev/null
	ln -s http-parser-2.7.1 http_parser
	cd http_parser && make && make parsertrace && make url_parser &&make package

clean:
	rm -rf http-parser-2.7.1
	rm -rf http_parser
	rm -rf libhttp_parser.a

