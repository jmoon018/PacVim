TARGET     =  pacvim
PREFIX    ?=  /usr/local
BINDIR     =  $(PREFIX)/bin
MAPDIR     =  $(PREFIX)/share/pacvim-maps
OBJS      :=  $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
MAPS      :=  $(wildcard maps/*)
CXX       ?=  g++
CXXFLAGS  +=  -std=c++11 -DMAPS_LOCATION='"$(MAPDIR)"'
LDLIBS    +=  -lncurses -lpthread

ifneq ($(shell uname -s 2>/dev/null || echo nop),Darwin)
# OS has POSIX threads in libc
CXXFLAGS += -pthread
endif

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(LDLIBS) $^ -o $@

install: $(TARGET)
	install -Dm755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)
	install -d $(DESTDIR)$(MAPDIR)
	install -t $(DESTDIR)$(MAPDIR) $(MAPS)

uninstall:
	$(RM) $(DESTDIR)$(BINDIR)/$(TARGET)
	$(RM) -r $(DESTDIR)$(MAPDIR)

.PHONY: install uninstall
