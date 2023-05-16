all: bgame

.SILENT: all bgame clean

bgame: controller.c logging.c message.c
	gcc controller.c logging.c message.c -g -o bgame

clean:
	rm -f bgame

pack:
	rm -f hw1.tar.gz
	tar czf hw1.tar.gz logging.c message.c controller.c Makefile