
TARGET = wsproxy
SRC = wsproxy.c
CFLAGS= -Wall

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lwebsockets

clean:
	rm -vf $(TARGET)
