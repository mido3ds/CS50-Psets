# compiler to use
CC = g++

# flags to pass compiler
CFLAGS = -ggdb3 -O0 -Qunused-arguments -std=c++14 -Wall -Werror

# name for executable
EXE = app

# space-separated list of header files
HDRS = 

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS = -lm

# space-separated list of source files
SRCS = quizz.cpp

# automatically generated list of object files
OBJS = $(SRCS:.cpp=.o)


# default target
$(EXE): $(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# dependencies 
$(OBJS): $(HDRS) Makefile

# housekeeping
clean:
	rm -f core $(EXE) *.o
