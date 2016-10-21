
name 	     = moonringplane
date 	     = $(shell echo `date '+%Y-%m-%d'`)
version      = 1.1
distname     = MRP-$(version)
src_distname = $(distname)-src

DIRS = midicl seq shell gshell


all: dirs



dirs:
	for x in $(DIRS); do cd $$x; make; cd ..; done



tags: always
	find . -name '*.c' -or -name '*.cpp' -or -name '*.h' -exec etags -a {} \;


line-count lines:
	find . -name '*.c' -or -name '*.cpp' -or -name '*.h' -or -name '*.xml' | xargs wc -l



backup bak: clean
	cd .. && tar cvO $(name) | gzip -c > $(name)-$(date).tar.gz


#
# to build the distro, do make dist.  note that we make the
# dist depend on the static qt lib, to make sure that the
# dist gets it statically linked.
#
dist: binary-dist source-dist

binary-dist: $(PROG) always $(QTDIR)/lib/libqt.a
	rm -rf dist/binary/$(distname)
	mkdir -p dist/binary/$(distname)
	cp -pR "gshell/bin/MRP Sequencer.app" dist/binary/$(distname)
	cp shell/bin/MRPShell dist/binary/$(distname)
	cp dist/binary/README dist/binary/$(distname)
	find dist/binary/$(distname) -name CVS -or -name '*~' -or -name .cvsignore -or -name .gdb_history | xargs -I {} rm -rf "{}"
	(cd dist/binary && tar cvO $(distname) | gzip -c > $(distname).tar.gz)


#
# this is a little grody, but checkout is the easiest way to
# get a clean tree
#
source-dist: always
	rm -rf dist/source/$(src_distname)
	cd dist/source && cvs co $(name)
	rm -rf dist/source/$(name)/jason
	rm -rf dist/source/$(name)/site
	rm -rf dist/source/$(name)/dist
	find dist/source/$(name) -name CVS -or -name .cvsignore | xargs -I {} rm -rf "{}"
	cd dist/source && mv $(name) $(src_distname)
	cd dist/source && tar cvO $(src_distname) | gzip -c > $(src_distname).tar.gz




#  removing deps like this is haxor - some problem with
#  the makefiles such that when you do a make clean they
#  look at the deps
clean:
	rm -f */dep/*.d
	for x in $(DIRS); do cd $$x; make clean; cd ..; done


always:
