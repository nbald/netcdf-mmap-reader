LDFLAGS =  -L. -lncmmap
#CXXFLAGS = -g -Wall -DDEBUG -I../include
CXXFLAGS = -g -Wall -O3 -I../include

all : test

libncmmap.so : ../include/OpenMeteoData/NcMmap.h NcMmap.cpp
	@rm -f libncmmap.so
	$(CXX) -fPIC -shared $(CXXFLAGS) NcMmap.cpp -o $@

test : libncmmap.so test.o 
	$(CXX) test.o -o $@ $(LDFLAGS)

install : libncmmap.so
	mkdir -p /usr/local/include/OpenMeteoData
	cp ../include/OpenMeteoData/NcMmap.h /usr/local/include/OpenMeteoData
	cp libncmmap.so /usr/local/lib
	
clean:
	@rm -f test
	@rm -f *.so.*
	@rm -f *.o