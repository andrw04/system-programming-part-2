encryptor: main.o file_utils.o caesar.o
	gcc -o encryptor main.o file_utils.o caesar.o

main.o: main.c
	gcc -c main.c

file_utils.o: file_utils.c
	gcc -c file_utils.c

caesar.o: caesar.c
	gcc -c caesar.c

clean:
	rm -f *.o