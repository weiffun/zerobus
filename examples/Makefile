CC = gcc
CPP = g++

CFLAGS = -g -Wall -fPIC -I../src/zmqbind -I../src/framework
LIBS =  -L../lib -Wl,-Bstatic -lzmqbind  -Wl,-Bdynamic -lzmq 
LIBS2 = -L../lib -Wl,-Bstatic -lzerobus -L../lib -Wl,-Bstatic -lzmqbind  -Wl,-Bdynamic -lzmq 
LIBS3 =  -L../lib -Wl,-Bstatic -lzerobus  -L../lib -Wl,-Bstatic -lzmqbind -L../lib -Wl,-Bstatic -ltinyxml -Wl,-Bdynamic -lzmq 
 
C_FILE = $(wildcard ./*.c)
CPP_FILE = $(wildcard ./*.cpp)
O_CC_FILE = $(patsubst ./%.c, %.o, $(C_FILE))
O_CXX_FILE = $(patsubst ./%.cpp, %.o, $(CPP_FILE))
O_ALL_OBJS = $(O_CXX_FILE) $(O_CC_FILE)

%.o : %.cpp
	$(CPP) -o $@ $(CFLAGS)  -c  $<

%.o : %.c
	$(CPP) -o $@ $(CFLAGS)  -c  $<
	
all : echoclient echoserver testchannelclient testchannelserver testchannelconfigclient  testchannelconfigsrv

echoclient : echoclient.o
	$(CPP) -o $@ $< $(LIBS)
echoserver : echoserver.o
	$(CPP) -o $@ $< $(LIBS)
	
testchannelclient : testchannelclient.o
	$(CPP) -o $@ $< $(LIBS2)

testchannelserver : testchannelserver.o
	$(CPP) -o $@ $< $(LIBS2)
	
testchannelconfigclient : testchannelconfigclient.o
	$(CPP) -o $@ $< $(LIBS3)

testchannelconfigsrv : testchannelconfigsrv.o
	$(CPP) -o $@ $< $(LIBS3)
	
clean:
	rm -f *.o 