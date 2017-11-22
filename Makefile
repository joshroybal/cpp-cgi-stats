cppstats.cgi: main.o stats.o
	g++ -static -s -o cppstats.cgi main.o stats.o

main.o: main.cpp stats.hpp
	g++ -O2 -c main.cpp

stats.o: stats.cpp stats.hpp
	g++ -O2 -c stats.cpp

install:
	sudo cp cppstats.html /srv/httpd/htdocs
	sudo chown apache:apache /srv/httpd/htdocs/cppstats.html
	sudo cp cppstats.cgi /srv/httpd/cgi-bin
	sudo chown apache:apache /srv/httpd/cgi-bin/cppstats.cgi

clean:
	rm cppstats.cgi && rm *.o

