#
#  make dirs if needed
#
$(shell mkdir -p obj lib dep)



LIB  = lib/libseq.a


#
#  default target
#
all: dep $(PROG) $(LIB)

SOURCES = \
	MRPEvent.cpp \
	MRPEventStatus.cpp \
	MRPMIDIClock.cpp \
	MRPSelectionAction.cpp \
	MRPSelectionFitToKey.cpp \
	MRPSequencer.cpp \
	MRPSequencerClock.cpp \
	MRPSequencerListener.cpp \
	MRPTimeManagerClock.cpp \
	MRPTrack.cpp

HEADERS =  \
	MRPEvent.h \
	MRPEventStatus.h \
	MRPMIDIClock.h \
	MRPSelectionAction.h \
	MRPSelectionFitToKey.h \
	MRPSequencer.h \
	MRPSequencerClock.h \
	MRPTimeManagerClock.h \
	MRPTrack.h

include ../common.makefile

CPPFLAGS += -I../midicl -I../shell

lib: $(LIB)

$(LIB): $(OBJECTS)
	$(LIBTOOL) $^ -o $@

clean:
	rm -f $(PROG) $(LIB) obj/*.o dep/*.d

