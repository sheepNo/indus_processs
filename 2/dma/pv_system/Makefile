SRCS = ../models/pv_dma.cpp ../models/pv_memory.cpp dma_testbench.cpp main.cpp
EXEC = simudma
OBJS = $(SRCS:.cpp=.o)
FLAGS = -DPV_MEMORY_DEBUG -DPV_DMA_DEBUG -DPV_ROUTER_DEBUG -DPV_DMA_BLOCK_TRANSFER 
DIR = directory where SystemC is installed
DIR = /home/p/pierrlau/SystemC/systemc-2.3.0
DIRTLM = /home/p/pierrlau/SystemC/TLM-2.0-Draft1/tlm

all : $(EXEC)
	@echo The executable is: $(EXEC)

$(EXEC) : $(OBJS)
	g++ -L$(DIR)/lib-linux64 $(OBJS) -lsystemc -o $(EXEC)

.cpp.o : 
	g++ -c $(FLAGS) -I$(DIR)/include -I$(DIRTLM) -I../models -I../protocol $< -o $@

clean:	
	-rm *.o *~ ../models/*.o ../models/*~
