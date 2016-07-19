# Automatically Generated Makefile by EDE.
# For use with: make
# Relative File Name: Makefile
#
# DO NOT MODIFY THIS FILE OR YOUR CHANGES MAY BE LOST.
# EDE is the Emacs Development Environment.
# http://cedet.sourceforge.net/ede.shtml
#

top="$(CURDIR)"/
ede_FILES=Project.ede Makefile

test-pwm_SOURCES=test-pwm.cpp dimmer_ctl.cpp GPIO.cpp
test-pwm_OBJ= test-pwm.o dimmer_ctl.o GPIO.o
CXX= g++
CFLAGS=-O3
CXX_COMPILE=$(CXX) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
CXX_DEPENDENCIES=-Wp,-MD,.deps/$(*F).P
CXX_LINK=$(CXX) $(CFLAGS) $(LDFLAGS) -L.
LDDEPS=-pthread -lprussdrv -lmosquittopp
VERSION=1.0
DISTDIR=$(top)test-pwm-$(VERSION)
top_builddir =

DEP_FILES=.deps/test-pwm.P .deps/dimmer_ctl.P .deps/GPIO.P

all: test-pwm

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)
-include $(DEP_FILES)

%.o: %.cpp
	@echo '$(CXX_COMPILE) -c $<'; \
	$(CXX_COMPILE) $(CXX_DEPENDENCIES) -o $@ -c $<

test-pwm: $(test-pwm_OBJ)
	$(CXX_LINK) -o $@ $^ $(LDDEPS)

tags:


clean:
	rm -f *.mod *.o *.obj .deps/*.P .lo

.PHONY: dist

dist:
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	cp $(test-pwm_SOURCES) $(ede_FILES) $(DISTDIR)
	tar -cvzf $(DISTDIR).tar.gz $(DISTDIR)
	rm -rf $(DISTDIR)


# End of Makefile
