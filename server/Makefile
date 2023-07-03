TAR=./s_v1
SRC=$(wildcard ./*.c)
CC=gcc
override CFLAGS+=-I. -lpthread

$(TAR):$(SRC)
	$(CC) $(^) -o $(@) $(CFLAGS)

clean:
	$(RM) $(TAR)

.PHONY:clean