#
#  make dirs if needed
#
$(shell mkdir -p obj bin dep)

PROG = Destinations MoogTest monitor Ports

include ../common.makefile

CPPFLAGS += -I../midicl

#
#  default target
#
all: dep $(PROG)

bin: $(PROG)

Destinations: obj/Destinations.o ../midicl/lib/libmidicl.a
	$(CXX) $(LDFLAGS) $^ -o bin/Destinations

bin/Destinations: obj/Destinations.o ../midicl/lib/libmidicl.a
	$(CXX) $(LDFLAGS) $^ -o $@

monitor: obj/monitor.o ../midicl/lib/libmidicl.a
	$(CXX) $(LDFLAGS) $^ -o bin/monitor

bin/monitor: obj/monitor.o ../midicl/lib/libmidicl.a
	$(CXX) $(LDFLAGS) $^ -o $@

processor: obj/processor.o ../midicl/lib/libmidicl.a
	$(CXX) $(LDFLAGS) $^ -o bin/processor

bin/processor: obj/processor.o ../midicl/lib/libmidicl.a
	$(CXX) $(LDFLAGS) $^ -o $@

MoogTest: obj/MoogTest.o ../midicl/lib/libmidicl.a
	$(CXX) $(LDFLAGS) $^ -o bin/MoogTest

bin/MoogTest: obj/MoogTest.o ../midicl/lib/libmidicl.a
	$(CXX) $(LDFLAGS) $^ -o $@

Ports: obj/Ports.o ../midicl/lib/libmidicl.a
	$(CXX) $(LDFLAGS) $^ -o bin/Ports

bin/Ports: obj/Ports.o ../midicl/lib/libmidicl.a
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(PROG) obj/*.o dep/*.d

