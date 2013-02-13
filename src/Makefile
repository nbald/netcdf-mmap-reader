LDFLAGS =  -L. -lncmmap
#CXXFLAGS = -g -Wall -DDEBUG -I../include
CXXFLAGS = -g -Wall -O3 -I../include

all : test

libncmmap.so : ../include/NcMmap.h NcMmap.cpp
	@rm -f libncmmap.so
	$(CXX) -fPIC -shared $(CXXFLAGS) NcMmap.cpp -o $@

test : libncmmap.so test.o 
	$(CXX) test.o -o $@ $(LDFLAGS)

install : libncmmap.so
	cp ../include/NcMmap.h /usr/include/
	cp libncmmap.so /usr/lib/
	
clean:
	@rm -f test
	@rm -f *.so.*
	@rm -f *.o