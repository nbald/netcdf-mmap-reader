LDFLAGS =  -L. -lncmmap
CXXFLAGS = -g -Wall -DDEBUG -I../include

all : test

libncmmap.so : ../include/OpenMeteoData/NcMmap.h NcMmap.cpp
	@rm -f libncmmap.so
	$(CXX) -fPIC -shared $(CXXFLAGS) NcMmap.cpp -o $@

test : libncmmap.so test.o 
	$(CXX) test.o -o $@ $(LDFLAGS)

clean:
	@rm -f test
	@rm -f *.so.*
	@rm -f *.o