#
#  make dirs if needed
#
$(shell mkdir -p obj lib dep)


#  can't get app to launch when executable name contains
#  space...
execname   = MRPSequencer
bundlename = MRP\ Sequencer

PROG = bin/$(bundlename).app/Contents/MacOS/$(execname)


date     = $(shell echo `date '+%Y-%m-%d'`)


LIB  = lib/libgshell.a


#
#  default target
#
all: dep $(PROG) $(LIB)



SOURCES =  ButtonBar.cpp \
           ChannelLabel.cpp \
	   CommandButton.cpp \
           ControlBar.cpp \
           CornerHeader.cpp \
	   GateLabel.cpp \
           LabelButton.cpp \
	   MuteLabel.cpp \
	   NoteLabel.cpp \
           NumEdit.cpp \
           SequencerWindow.cpp \
           ShellTextEdit.cpp \
           Step.cpp \
           StepHeader.cpp \
           StepHeaderRow.cpp \
	   StepLabel.cpp \
           Theme.cpp \
           Track.cpp \
           TrackHeader.cpp \
           VelocityLabel.cpp

HEADERS =  common.h



MOC_HEADERS = CommandButton.h \
              LabelButton.h \
              NumEdit.h \
              SequencerWindow.h \
	      ShellTextEdit.h


#
#  this creates some vars and rules
#
include ../common.makefile



CPPFLAGS += -I$(QTDIR)/include -I../midicl -I../seq -I../shell -I.


LDFLAGS += -framework Carbon -L$(QTDIR)/lib -lqt -lz
-LDFLAGS += -framework Carbon -L$(QTDIR)/lib -lqt-mt -lz



bin: $(PROG)


lib: $(LIB)


LOCAL_LIBS = ../midicl/lib/libmidicl.a ../seq/lib/libseq.a ../shell/lib/libshell.a


$(PROG): obj/main.o $(LIB) $(LOCAL_LIBS)
	$(CXX) $(LDFLAGS) $(LIB) $(LOCAL_LIBS) obj/main.o -o "$@"


$(LIB): $(OBJECTS) $(MOC_OBJECTS)
	$(LIBTOOL) $^ -o $@


clean:
	rm -f $(PROG) $(LIB) obj/*.o dep/*.d moc_*.cpp


test t: $(PROG)
	open bin/$(bundlename).app


always:
