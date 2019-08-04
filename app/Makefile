CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

.PHONY: clean all

all: sample1 sample2

sample1: sample1.c
	nfk -Lu sample1.c > sample12.c
	rm -f sample1.c
	mv sample12.c sample1.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

sample2: sample2.c
	nfk -Lu sample2.c > sample22.c
	rm -f sample2.c
	mv sample22.c sample2.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f sample1 sample1.d sample2 sample2.d