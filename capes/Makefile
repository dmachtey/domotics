top="$(CURDIR)"/
half-in-half-out-00A0_dtbo_SOURCES=half-in-half-out.dts
DSC=dsc
#C_LINK=$(CC) $(CFLAGS) $(LDFLAGS) -L.
#LD=ld
#LD_LINK=$(LD) $(LDFLAGS) -L.
#F90_LINK=$(F90) $(CFLAGS) $(LDFLAGS) -L.
VERSION=1.0
DISTDIR=$(top)cape-$(VERSION)
top_builddir =

DEP_FILES=.deps/half-in-half-out.P

all: half-in-half-out-00A0.dtbo

half-in-half-out-00A0.dtbo: $(half-in-half-out-00A0_dtbo_SOURCES)
	dtc -O dtb -o $(basename .dts $@).dtbo -b 0 -@ $<

tags:


clean:
	rm -f *.mod *.o *.obj .deps/*.P .lo *.dtbo

.PHONY: dist install

install:
	install -m 644 *.dtbo /lib/firmware/

dist:
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	cp $(half-in-half-out-00A0_dtbo_SOURCES) $(ede_FILES) $(DISTDIR)
	tar -cvzf $(DISTDIR).tar.gz $(DISTDIR)
	rm -rf $(DISTDIR)




# End of Makefile
