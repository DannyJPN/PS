include ../makefile.vars

NUM=6
MEMTYPE=ani
CUDA_NAME=cuda$(NUM)_$(MEMTYPE)
MAIN_NAME=main$(NUM)_$(MEMTYPE)

all: $(CUDA_NAME)

%.o: %.cu
	nvcc -g -c $<
	
$(CUDA_NAME).o: $(CUDA_NAME).cu $(wildcard *.h)

$(CUDA_NAME): $(MAIN_NAME).cpp uni_mem_allocator.cpp $(CUDA_NAME).o $(wildcard *.h) 
	g++ -g $(filter %.cpp %.o, $^) $(OPENCVHDRS) $(CUDALIBS) $(OPENCVLIBS) -o $@

clean:
	rm -f *.o $(CUDA_NAME)


