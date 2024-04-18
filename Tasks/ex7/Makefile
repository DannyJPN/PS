PATH:=${PATH}:/usr/local/cuda-10.2/bin
CC:=nvcc
OPT:=-Xcompiler -fPIC

main_md5: md5.o main_md5.o
	${CC} $^ -o $@

md5.o: md5.cu
	${CC} ${OPT}  -c $^  -o $@

main_md5.o: main_md5.cpp
	${CC} ${OPT}  -c $^  -o $@

clean:
	rm *.o main_md5
