BINS       = demo
OBJS	   = 
HDRS	   =

CGBIN      = ../bin
CGLIB	   = ../lib
CGINCLUDE  = ../include
INCLUDE	   = -I$(CGINCLUDE)

LIBCG      = -L$(CGLIB)/ -lmesh -lalgebra -lrply

LIBGL      = -L/usr/X11R6/lib -lX11 -lGL -lglut

#uncomment this line if MingW
#LIBGL     =  -lopengl32  -lglut32 

LIBUNIX	    = 

LIBS = $(LIBCG) $(LIBGL) $(LIBUNIX)

CC = gcc

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

CFLAGS = -g  $(INCLUDE)

all: $(BINS)

demo: demo.o
	g++ -o demo demo.o  $(LIBS) $(CFLAGS)

install: $(BINS)
	mv $(BINS) $(CGBIN)

clean:
	rm -f *.o *.h *~ core

