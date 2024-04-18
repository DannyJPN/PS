PATH:=${PATH}:/usr/local/cuda-10.2/bin
CC:=nvcc
OPT:=-Xcompiler -fPIC

main2: cuda2.o main2.o
	${CC} $^ -o $@

cuda2.o: cuda2.cu
	${CC} ${OPT}  -c $^  -o $@

main2.o: main2.cpp
	${CC} ${OPT}  -c $^  -o $@

clean:
	rm *.o main2
