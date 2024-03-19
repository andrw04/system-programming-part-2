sort: main.o merge_sort.o utils.o
	gcc -o sort main.o merge_sort.o utils.o -pthread

main.o: main.c
	gcc -c main.c -pthread

merge_sort.o: merge_sort.c
	gcc -c merge_sort.c -pthread

utils.o: utils.c
	gcc -c utils.c -pthread

clean:
	rm -f *.o