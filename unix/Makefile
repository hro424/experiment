
CC = clang

all: server client

SERVER_SRC = dgram_server.c
CLIENT_SRC = dgram_client.c

server: $(SERVER_SRC)
	$(CC) -o $@ $^ -lpthread

client: $(CLIENT_SRC)
	$(CC) -o $@ $^

clean:
	rm -vf server client
