#
#
#  some common compile parameters
#
#
PROF     = #-pg
OPT      = -g   
CPPFLAGS = $(OPT) $(PROF) -Wall
LDFLAGS  = -lm  -framework AudioToolbox -framework CoreServices -framework CoreMIDI
LIBTOOL  = /usr/bin/libtool
MOC      = $(QTDIR)/bin/moc


#
#
#  generate OBJECTS from SOURCES,
#           SOURCE_FILES from SOURCES,
#           HEADER_FILES from HEADERS,
#           DEP_FILES from SOURCES
#           MOC_OBJECTS from MOC_HEADERS
#
#
OBJECTS      = $(addprefix obj/, $(patsubst %.cpp,%.o, $(SOURCES)))

SOURCE_FILES = $(SOURCES)

HEADER_FILES = $(HEADERS)

DEP_FILES    = $(addprefix dep/, $(patsubst %.cpp,%.d, $(SOURCES)))

MOC_OBJECTS  = $(addprefix obj/, $(patsubst %.h,moc_%.o, $(MOC_HEADERS)))




#
#
#  some common rules
#
#
obj/%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@


dep/%.d: %.cpp
	$(CXX) $(CPPFLAGS) -MM -MT obj/$*.o  $< -o $@


moc_%.cpp: %.h
	$(MOC) $< -o $@


#obj/%.o: dep/%.d



#
#  include local deps
#
-include dep/*.d



#
#  some common targets
#
dep: $(DEP_FILES)

