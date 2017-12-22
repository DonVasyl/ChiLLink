all: chillink

CC=i686-w64-mingw32-gcc.exe
PATH:=$(PATH):/c/mingw-w64/i686-6.1.0-posix-dwarf-rt_v5-rev0/mingw32/bin

DATE_D = $(shell date +"%-d")
DATE_M = $(shell date +"%-m")
DATE_Y = $(shell date +"%-Y")

OBJS = \
	cl_main.o \
	cl_errors.o \
	cl_args.o \
	cl_handlers.o

chillink: $(OBJS)
	$(CC) -o chillink $(OBJS)

%.o: %.c
	$(CC) -O2 -D _POSIX -pedantic -std=gnu99 \
	-D CT_BDATE_D=$(DATE_D) \
	-D CT_BDATE_M=$(DATE_M) \
	-D CT_BDATE_Y=$(DATE_Y) \
	-c $< -o $@

clean:
	rm -f chillink
	rm -f *.o

install: chillink
	cp ./chillink /bin/
