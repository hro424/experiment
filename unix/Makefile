
CC = clang
CFLAGS = -Wall -Wextra -g

all: udp_server udp_client tcp_server tcp_client thread_server

UDP_SERVER_SRC = dgram_server.c
UDP_CLIENT_SRC = dgram_client.c
TCP_SERVER_SRC = stream_server.c
TCP_THREAD_SERVER_SRC = thread_server.c
TCP_CLIENT_SRC = stream_client.c

udp_server: $(UDP_SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

udp_client: $(UDP_CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $^

tcp_server: $(TCP_SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

thread_server: $(TCP_THREAD_SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

tcp_client: $(TCP_CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -vf udp_server udp_client
