all: hw2

.SILENT: all hw2 clean

hw2: hw2.cpp hw2_output.c 
	g++ hw2.cpp hw2_output.c -lpthread -o hw2 


clean:
	rm -f hw2

pack:
	rm -f hw2.tar.gz
	tar czf hw2.tar.gz hw2.cpp hw2_output.c hw2_output.h Makefile
