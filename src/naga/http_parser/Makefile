.PHONY:all


all:http-parser.2.7.1 libhttp_parser.a

libhttp_parser.a:
	ln -s  http-parser-2.7.1/libhttp_parser.a libhttp_parser.a

http-parser.2.7.1:
	tar xvf http-parser-2.7.1.tar.gz >> /dev/null
	ln -s http-parser-2.7.1 http-parser
	cd http-parser && make >>/dev/null && make parsertrace >>/dev/null && make url_parser >>/dev/null &&make package >>/dev/null

clean:
	rm -rf http-parser-2.7.1
	rm -rf http-parser
	rm -rf libhttp_parser.a

