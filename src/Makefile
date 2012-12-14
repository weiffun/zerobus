DIRS = zmqbind tinyxml framework 

OUTPUT_LIB = ../lib
OUTPUT_INCLUDE = ../include

all:
	for i in $(DIRS); do \
		(cd $$i && $(MAKE)) || exit 1; \
	done
	
install:
	-if [ ! -d $(OUTPUT_LIB) ]; then mkdir -p $(OUTPUT_LIB); fi;
	
	-if [ ! -d $(OUTPUT_INCLUDE) ]; then mkdir -p $(OUTPUT_INCLUDE); \
		mkdir -p $(OUTPUT_INCLUDE)/zmqbind;\
		mkdir -p $(OUTPUT_INCLUDE)/tinyxml;\
	fi;
	
	for i in $(DIRS); do \
		(cd $$i && $(MAKE) install) || exit 1; \
	done

clean:
	for i in $(DIRS); do \
		(cd $$i && $(MAKE) clean) || exit 1; \
	done
