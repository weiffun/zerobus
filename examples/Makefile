CC = gcc
CPP = g++

#CFLAGS = -g -Wall -fPIC -I../src/zmqbind -I../src/framework
CFLAGS = -g -Wall -fPIC -I../include 
#LIBS =  -L../lib -Wl,-Bstatic -lzmqbind  -Wl,-Bdynamic -lzmq 
#LIBS2 = -L../lib -Wl,-Bstatic -lzerobus -L../lib -Wl,-Bstatic -lzmqbind  -Wl,-Bdynamic -lzmq 
LIBS =  -L../lib -Wl,-Bstatic -lzerobus  -L../lib -Wl,-Bstatic -lzmqbind -L../lib -Wl,-Bstatic -ltinyxml -Wl,-Bdynamic -lzmq 
 
C_FILE = $(wildcard ./*.c)
CPP_FILE = $(wildcard ./*.cpp)
O_CC_FILE = $(patsubst ./%.c, %.o, $(C_FILE))
O_CXX_FILE = $(patsubst ./%.cpp, %.o, $(CPP_FILE))
O_ALL_OBJS = $(O_CXX_FILE) $(O_CC_FILE)

%.o : %.cpp
	$(CPP) -o $@ $(CFLAGS)  -c  $<

%.o : %.c
	$(CPP) -o $@ $(CFLAGS)  -c  $<
	
all : testchannelconfigclient  testchannelconfigsrv
	
testchannelconfigclient : testchannelconfigclient.o
	$(CPP) -o $@ $< $(LIBS)

testchannelconfigsrv : testchannelconfigsrv.o
	$(CPP) -o $@ $< $(LIBS)
	
clean:
	rm -f *.o 