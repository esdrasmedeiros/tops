SHELL = /bin/sh

DIRS =  algebra tops rply demo

all:
	for d in $(DIRS); do \
	  cd $$d; echo $$d; make; cd ..;\
	done

install:
	for d in $(DIRS); do \
	  cd $$d; echo $$d; make; make install; cd ..;\
	done

clean:
	for d in $(DIRS); do \
	  cd $$d; make clean; cd ..;\
	done

realclean: clean
	cd include; rm -f *.h; cd ..
	cd lib; rm -f *.a; cd ..
	cd bin; rm -f *; cd ..


