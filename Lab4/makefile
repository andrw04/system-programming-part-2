mydaemon: main.o file_utils.o
	gcc -o mydaemon main.o file_utils.o

main.o: main.c main.h
	gcc -c main.c

file_utils.o: file_utils.c
	gcc -c file_utils.c

clean:
	rm -f *.o