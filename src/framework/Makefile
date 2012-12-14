include ../Make.defines

LIBS +=  -L../zmqbind -lzmqbind   -L../tinyxml -ltinyxml

INC = -I.. -I.

TAEGET = libzerobus.a
TARGETSO = libzerobus.so
PROJECT = zmqbind_framework

C_FILE = $(wildcard ./*.c)
CPP_FILE = $(wildcard ./*.cpp)
O_CC_FILE = $(patsubst ./%.c, %.o, $(C_FILE))
O_CXX_FILE = $(patsubst ./%.cpp, %.o, $(CPP_FILE))
O_ALL_OBJS = $(O_CXX_FILE) $(O_CC_FILE)

%.o : %.cpp
	$(CPP) -o $@ $(CFLAGS) $(INC) -c  $<

%.o : %.c
	$(CPP) -o $@ $(CFLAGS) $(INC)  -c  $<

all : $(TAEGET)

$(TAEGET) : $(O_ALL_OBJS)
	rm -f $@ $(TARGETSO)
	ar rcs $@ $(O_ALL_OBJS) 
	$(CPP) -shared -o $(TARGETSO)  $(LIBS)  $(O_ALL_OBJS) 
	
install:
	cp $(TAEGET) ../../lib
	cp $(TARGETSO) ../../lib
	cp ZBus.hpp ../../include
	cp Channel.hpp ../../include
	cp Reactor.hpp ../../include
	cp CommonDefine.hpp ../../include
	cp Error.hpp ../../include
	cp ConfigParser.hpp ../../include
	
clean:
	rm -f *.o 
	rm -f $(TAEGET) $(TARGETSO)