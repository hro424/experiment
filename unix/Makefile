
CC = clang
CXX = clang++

CFLAGS = -Wall -Wextra -g
CXXFLAGS = -Wall -Wextra -g -std=c++11 -pthread

all: udp_server udp_client tcp_server tcp_client thread_server tcpd

UDP_SERVER_SRC = dgram_server.c
UDP_CLIENT_SRC = dgram_client.c
TCP_SERVER_SRC = stream_server.c
TCP_THREAD_SERVER_SRC = thread_server.c
TCP_CLIENT_SRC = stream_client.c
TCPD_SRC = tcpd.cc

udp_server: $(UDP_SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

udp_client: $(UDP_CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $^

tcp_server: $(TCP_SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^

thread_server: $(TCP_THREAD_SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

tcp_client: $(TCP_CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $^

tcpd: $(TCPD_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -vf udp_server udp_client tcp_server tcp_client thread_server tcpd

