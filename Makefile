stats.cgi: cgistats.o statistics.o
	g++ -static -s -o stats.cgi cgistats.o statistics.o

cgistats.o: cgistats.cpp statistics.hpp
	g++ -O2 -c cgistats.cpp

statistics.o: statistics.cpp statistics.hpp
	g++ -O2 -c statistics.cpp

install:
	sudo cp stats.cgi /srv/httpd/cgi-bin
	sudo chown apache:apache /srv/httpd/cgi-bin/stats.cgi

clean:
	rm stats.cgi && rm *.o
