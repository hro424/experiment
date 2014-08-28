
TARGET = md
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $< -Wall -O2 -o $@

.PHONY: clean
clean:
	rm -vf $(TARGET)
