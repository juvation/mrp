#
#  make dirs if needed
#
$(shell mkdir -p obj lib bin dep)

LIB = lib/libmidicl.a


#
#  default target
#
all: dep $(PROG) $(LIB)



SOURCES = MIDICLClient.cpp \
          MIDICLChannelisingListener.cpp \
          MIDICLDestination.cpp \
          MIDICLEchoingListener.cpp \
          MIDICLInputPort.cpp \
          MIDICLInputPortListener.cpp \
          MIDICLMonitor.cpp \
					MIDICLOutputPort.cpp \
					MIDICLProcessingListener.cpp


HEADERS =  MIDICLClient.h \
          MIDICLChannelisingListener.h \
	   MIDICLDestination.h \
				MIDICLEchoingListener.h \
	   MIDICLInputPort.h \
	   MIDICLInputPortListener.h \
	   MIDICLMonitor.h \
	   MIDICLOutputPort.h \
					MIDICLProcessingListener.h



include ../common.makefile



lib: $(LIB)




$(LIB): $(OBJECTS)
	$(LIBTOOL) $^ -o $@


clean:
	rm -f $(LIB) obj/*.o dep/*.d




