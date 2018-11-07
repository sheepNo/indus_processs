SRCS = fifo_perf.cpp
EXEC = fifo_perf
OBJS = $(SRCS:.cpp=.o)
DIR = /home/p/pierrlau/SystemC/systemc-2.3.0

all : $(EXEC)
	@echo The resulting executable is: $(EXEC)

$(EXEC) : $(OBJS)
	g++ -L$(DIR)/lib-linux64 $(OBJS) -lsystemc -o $(EXEC)

.cpp.o : 
	g++ -c -I$(DIR)/include $<

clean:
	-rm -i *.o *~ 

