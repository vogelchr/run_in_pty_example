CFLAGS=-Wall -Os -ggdb

all : run_in_vty

run_in_vty : run_in_vty.o
	$(CC) -o $@ $^

.PHONY : clean
clean :
	rm -f *~ *.o run_in_vty
