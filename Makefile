CC := gcc
AR := ar
RM := rm

TTREE_OBJS := ttree.o
CFLAGS := -g

all: ttree.a

ttree.a: $(TTREE_OBJS)
	$(AR) rc $@ $^

%.c: %.o
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) -f *.o
	$(RM) ttree.a