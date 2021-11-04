CC 		= gcc
OBJS	= fraction_reduce.o input.o fraction.o
CFLAGS	= -O2
BIN		= rref

all: $(BIN)

rref: $(OBJS)
	$(CC) $(CFLAGS) ${OBJS} -o rref

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r rref *.o *.dSYM