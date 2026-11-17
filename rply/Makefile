INCLUDE =../include
LIBDIR = ../lib

CC = gcc

CFLAGS = $(OPT) -Wall -I$(INCLUDE)

.c.o:
	$(CC) $(CFLAGS) -c $<

HEADERS = rply.h 

SOURCES = rply.c 

OBJECTS = rply.o

LIBNAME = librply.a


$(LIBNAME): $(HEADERS) $(OBJECTS)
	ar ruv $(LIBNAME) $(OBJECTS)
	ranlib $(LIBNAME)

$(OBJECTS):  $(HEADERS)

install: $(LIBNAME)
	mv $(LIBNAME) $(LIBDIR)
	cp $(HEADERS) $(INCLUDE)

clean:
	rm -f *.o *~ $(LIBNAME)




