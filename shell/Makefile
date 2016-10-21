#
#  make dirs if needed
#
$(shell mkdir -p obj lib bin dep)



PROG = bin/MRPShell

LIB  = lib/libshell.a


#
#  default target
#
all: dep $(PROG) $(LIB)



SOURCES = MRPShell.cpp \
          MRPShellInterface.cpp \
	  MRPStdioInterface.cpp


HEADERS =  MRPShell.h \
	   MRPShellInterface.h \
	   MRPStdioInterface.h


include ../common.makefile


CPPFLAGS += -I../midicl -I../seq

-LDFLAGS += -framework Carbon -L$(QTDIR)/lib -lqt -lz
+LDFLAGS += -framework Carbon -L$(QTDIR)/lib -lqt-mt -lz



bin: $(PROG)


lib: $(LIB)



$(PROG): obj/main.o $(LIB) ../midicl/lib/libmidicl.a ../seq/lib/libseq.a
	$(CXX) $(LDFLAGS) $^ -o $@



$(LIB): $(OBJECTS)
	$(LIBTOOL) $^ -o $@


clean:
	rm -f $(PROG) $(LIB) obj/*.o dep/*.d


test t: $(PROG)
	$(PROG)

