include ../Make.defines

CFLAGS += -fPIC

TAEGET = libzmqbind.a
TARGETSO = libzmqbind.so
PROJECT = zmqbind

C_FILE = $(wildcard ./*.c)
CPP_FILE = $(wildcard ./*.cpp)
O_CC_FILE = $(patsubst ./%.c, %.o, $(C_FILE))
O_CXX_FILE = $(patsubst ./%.cpp, %.o, $(CPP_FILE))
O_ALL_OBJS = $(O_CXX_FILE) $(O_CC_FILE)

%.o : %.cpp
	$(CPP) -o $@ $(CFLAGS)  -c  $<

%.o : %.c
	$(CPP) -o $@ $(CFLAGS)  -c  $<

all : $(TAEGET)

$(TAEGET) : $(O_ALL_OBJS)
	rm -f $@ $(TARGETSO)
	ar rcs $@ $(O_ALL_OBJS) 
	$(CPP) -shared -o $(TARGETSO)  $(LIBS)  $(O_ALL_OBJS) 

install:
	cp $(TAEGET) ../../lib
	cp $(TARGETSO) ../../lib
	cp *.hpp ../../include/zmqbind
clean:
	rm -f *.o 
	rm -f $(TAEGET) $(TARGETSO)