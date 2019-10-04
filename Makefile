CC := gcc
LD := $(CC)
CFLAGS := -O2 -Wall -pedantic -Werror -gxcoff -maix64
LDFLAGS := -maix64

PGM := libl
OBJ := qlichgll.o ebcdic.o main.o error.o
DEPS := qlichgll.h ebcdic.h error.h

.PHONY: all clean

all: $(PGM)

clean:
	rm -f $(OBJ) $(PGM) core

$(OBJ): %.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PGM): $(OBJ)
	$(LD) -o $@ $^ -L/QOpenSys/usr/lib -liconv $(LDFLAGS)
