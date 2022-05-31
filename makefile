# g++ main.cpp -o server -L /usr/lib -lboost_system -lboost_thread -lpthread

CORSS =
CC=$(CORSS)g++

EXE=main

DEFINES= 
LDFLAGS = -L /usr/lib -lboost_system -lboost_thread -lpthread
CFLAGSXX= -g -I./include $(DEFINES)

C_SRC=$(wildcard *.c)
CPP_SRC = $(wildcard *.cpp)
CC_SRC = $(wildcard *.cc)
SRC = $(C_SRC) $(CPP_SRC) $(CC_SRC)
OBJ = $(C_SRC:.c=.o) $(CC_SRC:.cc=.o) $(CPP_SRC:.cpp=.o)

defualt : $(EXE) clean

all : $(EXE)
	echo complete!!

$(EXE) : $(OBJ)
	$(CC) $(CFLAGSXX) -o $@ $^ $(LDFLAGS)
	strip $@
	

clean:
	rm *.o


