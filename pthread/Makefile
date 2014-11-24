TARGET := server

SRCS := Main.cc			\
	SocketServer.cc

OBJS := $(SRCS:.cc=.o)

CXX := clang++
CXXFLAGS := -Wall -g -std=c++11
LD := clang++
LDFLAGS := -lpthread

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -o $@ $(LDFLAGS) $^

%.o: %.cc
	$(CXX) -o $@ -c $(CXXFLAGS) $<

.PHONY: clean

clean:
	rm -vf $(TARGET) $(OBJS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^ > ./.depend

include .depend
