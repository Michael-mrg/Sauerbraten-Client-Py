CFLAGS  = -ggdb3 -W -Wall -Werror -pedantic -Wno-unused -std=c99
LDFLAGS = 
OBJS    = sauer.o main.o

CFLAGS  += -I/usr/include/python2.6
LDFLAGS += -lpython2.6

LDFLAGS += -lenet -lz

main: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $< -c $(CFLAGS)

.PHONY: clean
clean:
	rm -f main *.o
