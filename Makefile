TARGET     =  pacvim
PREFIX    ?=  /usr/local
BINDIR     =  $(PREFIX)/bin
MAPDIR     =  $(PREFIX)/share/pacvim-maps
OBJS      :=  $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
MAPS      :=  $(wildcard maps/*)
CXX       ?=  g++
CXXFLAGS  +=  -std=c++11 -DMAPS_LOCATION=\'"$(MAPDIR)"\'
LDLIBS    +=  -lncurses -lpthread

ifneq ($(shell uname -s 2>/dev/null || echo nop),Darwin)
# OS has POSIX threads in libc
CXXFLAGS += -pthread
endif

-include $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# Generate dependency files with correct target names
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -MT $@ -c $< -o $@

install: $(TARGET)
	install -Dm755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)
	install -d $(DESTDIR)$(MAPDIR)
	install -t $(DESTDIR)$(MAPDIR) $(MAPS)

uninstall:
	$(RM) $(DESTDIR)$(BINDIR)/$(TARGET)
	$(RM) -r $(DESTDIR)$(MAPDIR)

clean:
	$(RM) $(wildcard src/*.o) $(wildcard src/*.d) $(TARGET)

.PHONY: install uninstall clean
