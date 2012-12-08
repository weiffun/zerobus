DIRS = zmqbind  framework 

all:
	for i in $(DIRS); do \
		(cd $$i && $(MAKE)) || exit 1; \
	done

clean:
	for i in $(DIRS); do \
		(cd $$i && $(MAKE) clean) || exit 1; \
	done
