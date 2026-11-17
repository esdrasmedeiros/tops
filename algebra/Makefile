INCLUDE =../include
LIBDIR = ../lib

CC = gcc

CFLAGS = $(OPT) -Wall -I$(INCLUDE)

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

HEADERS = algebra3.h 

SOURCES = algebra3.cpp 

OBJECTS = algebra3.o

LIBNAME = libalgebra.a


$(LIBNAME): $(HEADERS) $(OBJECTS)
	ar ruv $(LIBNAME) $(OBJECTS)
	ranlib $(LIBNAME)

$(OBJECTS):  $(HEADERS)

install: $(LIBNAME)
	mv $(LIBNAME) $(LIBDIR)
	cp $(HEADERS) $(INCLUDE)

clean:
	rm -f *.o *~ $(LIBNAME)




