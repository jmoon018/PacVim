SRCS   = $(wildcard src/*.cpp)
OBJS   = $(SRCS:.cpp=.o)

PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
SHARE  = $(PREFIX)/share
MAPDIR = $(SHARE)/pacvim-maps

CXX    = g++
CFLAGS = -std=c++11 -DMAPS_LOCATION='"$(MAPDIR)"'
LFLAGS = -lncurses

TARGET = pacvim
MAPS   = maps
DEPS   = .deps

ifneq ($(shell uname -s 2>/dev/null || echo nop),Darwin)
# OS has POSIX threads in libc
CFLAGS += -pthread
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $^ $(LFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(DEPS):
	$(CXX) -M $(SRCS) >| $@

clean:
	$(RM) $(OBJS) $(DEPS)

install: $(TARGET)
	install -d $(BINDIR) $(SHARE)
	install -m 755 $(TARGET) $(BINDIR)/
	cp -r $(MAPS) $(MAPDIR)

uninstall:
	$(RM) $(BINDIR)/$(TARGET)
	$(RM) -r $(MAPDIR)

-include $(DEPS)
