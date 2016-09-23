#
# Makefile
#
# Computer Science 50
# Problem Set 5
#


# compiler to use
CC = clang

# flags to pass compiler
CFLAGS = -ggdb3 -O0 -Qunused-arguments -std=c11 -Wall -Werror

# name for executable
EXE = test

# space-separated list of header files
HDRS = trietable.h

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS = -lcs50

# space-separated list of source files
SRCS = test.c trietable.c

# automatically generated list of object files
OBJS = $(SRCS:.c=.o)


# default target
$(EXE): $(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# dependencies 
$(OBJS): $(HDRS) Makefile

# housekeeping
clean:
	rm -f core $(EXE) *.o
