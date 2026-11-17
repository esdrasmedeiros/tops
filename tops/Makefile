LIBDIR = ../lib
INCLUDE =../include

CC = gcc

CFLAGS = $(OPT) -Wall -I$(INCLUDE)

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

HEADERS = mesh.h 

SOURCES = surf.cpp  edge.cpp face.cpp stellar.cpp handle.cpp

OBJECTS = surf.o edge.o face.o stellar.o handle.o 


LIBNAME = libmesh.a


$(LIBNAME): $(HEADERS) $(OBJECTS)
	ar ruv $(LIBNAME) $(OBJECTS)
	ranlib $(LIBNAME)

$(OBJECTS):  $(HEADERS)

install: $(LIBNAME)
	mv $(LIBNAME) $(LIBDIR)
	cp $(HEADERS) $(INCLUDE)

clean:
	rm -f *.o *~ $(LIBNAME)



