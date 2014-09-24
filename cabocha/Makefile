
TARGET = cabochad
SRC = cabochad.c
CC=clang
CFLAGS= -Wall -I/usrlocal/include

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lwebsockets -lcabocha

clean:
	rm -vf $(TARGET)
