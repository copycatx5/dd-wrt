iperf-configure:
	cd iperf && ./configure --host=$(ARCH)-linux --prefix=/usr CC="$(CC)" CFLAGS="$(COPTS) -DNEED_PRINTF -D_GNU_SOURCE -ffunction-sections -fdata-sections -Wl,--gc-sections -Drpl_malloc=malloc" CPPFLAGS="$(COPTS) -DNEED_PRINTF -ffunction-sections -fdata-sections -Wl,--gc-sections -Drpl_malloc=malloc"

iperf:
	$(MAKE) -C iperf

iperf-clean:
	if test -e "iperf/Makefile"; then make -C iperf clean; fi
	@true

iperf-install:
	make -C iperf install-exec DESTDIR=$(INSTALLDIR)/iperf
	rm -f $(INSTALLDIR)/iperf/usr/lib/*.a
	rm -f $(INSTALLDIR)/iperf/usr/lib/*.la

