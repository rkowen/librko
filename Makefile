#
# Makefile - to build librko
#
# by R.K. Owen, Ph.D.
#
# do:	make
#	make install
#	make uninstall
#	make help - for further info

include Makefile.inc

LIB	=librko.a
SRCS	=invoke.c timedfgets.c rkoerror.c memory.c strmem.c wcstrcmp.c \
	 nameread.c foptim.c fzeroin.c divdiff.c urand.c strchop.c \
	 ansi_seq.c gethostbyX.c tcp_connect.c macrosub.c uvec.c \
	 strdbecpy.c istext.c list.c dirtree.c spawn.c gcd.c

# requires a GNU "make" for the following
LIBOBJS	=$(SRCS:%.c=$(LIB)(%.o))

OBJS	=$(SRCS:.c=.o)
DOCS	=$(SRCS:.c=.3)
DOCSL	=librko.3 $(DOCS)

.SUFFIXES: .3 .man

.c.a:
	$(CC) -c $(CFLAGS) $<
	$(AR) $(ARFLAGS) $@ $*.o
	-$(RM) -f $*.o

.o.a:
	$(AR) $(ARFLAGS) $@ $*.o
	-$(RM) -f $*.o

#.c.o :
#	$(CC) $(CFLAGS) -c $<

.man.3 :
	$(NROFF) $(MAN) $< > $@

all : $(LIB) $(DOCSL)

$(LIB): $(LIBOBJS)
	@echo lib is now up-to-date

#$(LIB) : $(OBJS) librko.h
#	-$(RM) $(LIB)
#	$(AR) $(ARFLAGS) $(LIB) $(OBJS)

librko.man : Librko.man whatis.librko
	$(SHELL) Librko.man

urand.o : urand/urand.o
	$(CP) urand/urand.o urand.o

urand/urand.o : urand/urand.c
	cd urand; ./script.csh "$(CC) $(CFLAGS)" ; $(CP) urand.o ..

urand.man : urand/urand.man
	cp urand/urand.man urand.man

install : installlib installdoc

installlib : $(LIB)
	$(INSTALL) $(LIB) $(LIBDIR) $(PERM) $(OWN) $(GRP)
	$(INSTALL) librko.h $(INCDIR) $(PERM) $(OWN) $(GRP)

installdoc : $(DOCSL)
	-for f in $(DOCSL); do \
		$(INSTALL) $$f $(MANDIR)/man3 $(PERM) $(OWN) $(GRP) ; done
	-for f in $(DOCSL); do xxx=`$(BSNM) $$f .3`; \
		$(INSTALL) $$xxx.man $(MANDIR)/cat3/$$f $(PERM) $(OWN) $(GRP);\
		done

uninstall : uninstalllib uninstalldoc

uninstalllib :
	-$(RM) $(LIBDIR)/$(LIB)
	-$(RM) $(INCDIR)/librko.h

uninstalldoc :
	-for f in $(DOCSL); do $(RM) $(MANDIR)/man3/$$f ; done
	-for f in $(DOCSL); do $(RM) $(MANDIR)/cat3/$$f ; done

tgz : clobber
	cd ..; tar -czvf librko.tgz librko

shar : clobber
	cd .. ; librko/etc/mkshar -s 250000 -f - librko > librko.shar

#
# the following are individual packages for distribution
#
alltgz : FTP/color.tgz FTP/mexpn.tgz FTP/netup.tgz FTP/memory.tgz \
	 FTP/istext.tgz

FTP/color.tgz : apps/color.c ansi_seq.c librko.h apps/color.man
	./setupmake color apps/color.c ansi_seq.c \
		librko.h apps/color.man
	tar -czvf FTP/color.tgz		color
	rm -rf				color

FTP/mexpn.tgz : apps/mexpn.c gethostbyX.c tcp_connect.c timedfgets.c \
		rkoerror.c librko.h apps/mexpn.man
	./setupmake mexpn apps/mexpn.c gethostbyX.c tcp_connect.c timedfgets.c \
		rkoerror.c librko.h apps/mexpn.man
	tar -czvf FTP/mexpn.tgz		mexpn
	rm -rf				mexpn

FTP/netup.tgz : apps/netup.c gethostbyX.c tcp_connect.c \
		rkoerror.c librko.h apps/netup.man
	./setupmake netup apps/netup.c gethostbyX.c tcp_connect.c \
		rkoerror.c librko.h apps/netup.man
	tar -czvf FTP/netup.tgz		netup
	rm -rf				netup

FTP/memory.tgz : memory.c librko.h memory.man
	./setupmake memory amemory.c librko.h memory.man
	tar -czvf FTP/memory.tgz	memory
	rm -rf				memory

FTP/istext.tgz : istext.c apps/istextmain.c apps/istext.man
	./setupmake istext istext.c apps/istextmain.c apps/istext.man
	tar -czvf FTP/istext.tgz	istext
	rm -rf				istext

#
# Helpful info
#
help :
	@$(ECHO) ""
	@$(ECHO) "make		- makes library & man pages"
	@$(ECHO) "make install	- installs library & man pages"
	@$(ECHO) "make uninstall	- uninstalls library & man pages"
	@$(ECHO) "make clean	- removes .o & executables"
	@$(ECHO) "make wipe	- does clean & removes most 'made' files"
	@$(ECHO) "make clobber	- does wipe & removes all 'made' files"
	@$(ECHO) "make tgz	- does clobber & makes ../librko.tgz distribution"
	@$(ECHO) "make alltgz	- makes all the FTP/*.tgz individual distributions"
	@$(ECHO) "make help	- this information"
	@$(ECHO) ""

#
# various clean-up targets
#

clean :
	-$(RM) -f a.out core *.o
	-cd test; $(RM) -f a.out core *.o

wipe : clean
	-$(RM) *.3 librko.man
	-cd urand; ./clean
	-cd metropolis; make clean
	-rm FTP/*.tgz

clobber : wipe
	-$(RM) $(LIB)
	-$(RM) urand/urand.o
	-cd metropolis; make clobber
