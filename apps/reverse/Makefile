# "%W%"
# 
# R.K.Owen,Ph.D.
# Sterling Software
# NASA/Ames Research Center MS 258-6
# Moffett Field, CA 94035-1000
# email: rkowen@ames.arc.nasa.gov or rkowen@nas.nasa.gov
#
# The following lines tailor the program & support files to the current
#   environment
#
ROOT	=/usr/local
BINDIR	=${ROOT}/bin
MANDIR	=${ROOT}/man
BIN	=rk
BINGRP	=owen
#
# 'make' makes the program
# 'make install' installs program and man pages
#

# requires an ANSI-C compiler
CC	=gcc
CP	=cp
MV	=mv
RM	=rm
MKDIR	=mkdir
CHMOD	=chmod
CHOWN	=chown
CHGRP	=chgrp
NROFF	=nroff
# the following refers to the UNICOS man macro package
#MAN	=-muc
MAN	=-man
SED	=sed
SHAR	=mkshar -f -
GNUTAR	=tar
#
#-----------------------------------------------------------------------------
# shouldn't have to make changes below this point
#-----------------------------------------------------------------------------
#

all:	reverse reverse.1

reverse:	reverse.o
	$(CC) -o reverse reverse.o

install: installprog installman

uninstall: uninstallprog uninstallman

installprog:
	-$(MV) ${BINDIR}/reverse ${BINDIR}/OLDreverse
	$(CP) reverse ${BINDIR}/reverse
	$(CHMOD) 0755 ${BINDIR}/reverse
	$(CHOWN) ${BIN} ${BINDIR}/reverse
	$(CHGRP) ${BINGRP} ${BINDIR}/reverse

installman: reverse.1 reverse.man
	-$(MKDIR) ${MANDIR}/cat1
	-$(MKDIR) ${MANDIR}/man1
	-$(CHMOD) 775 ${MANDIR}/cat1
	-$(CHMOD) 775 ${MANDIR}/man1
	-$(CHOWN) ${BIN} ${MANDIR}
	-$(CHGRP) ${BINGRP} ${MANDIR}
	-$(MV) ${MANDIR}/man1/reverse.1 ${MANDIR}/man1/OLDreverse.1
	-$(CP) reverse.man ${MANDIR}/man1/reverse.1
	-$(CHMOD) 0644 ${MANDIR}/man1/reverse.1
	-$(CHOWN) ${BIN} ${MANDIR}/man1/reverse.1
	-$(CHGRP) ${BINGRP} ${MANDIR}/man1/reverse.1
	-$(CP) reverse.1 ${MANDIR}/cat1/reverse.1
	-$(CHMOD) 0644 ${MANDIR}/cat1/reverse.1
	-$(CHOWN) ${BIN} ${MANDIR}/cat1/reverse.1
	-$(CHGRP) ${BINGRP} ${MANDIR}/cat1/reverse.1

uninstallman:
	-$(RM) ${MANDIR}/cat1/reverse.1
	-$(RM) ${MANDIR}/man1/reverse.1

uninstallprog:
	-$(RM) ${BINDIR}/reverse

reverse.1 : reverse.man
	$(NROFF) $(MAN) reverse.man > reverse.1
	$(CHMOD) 644 reverse.1

reverse.man : Reverse.man
	$(SED)  -e "s:BINDIR:$(BINDIR):" \
		-e "s:MANDIR:$(MANDIR):" \
		Reverse.man  > reverse.man
	$(CHMOD) 644 Reverse.man reverse.man

shar : clobber
	cd ..; $(SHAR) reverse > reverse.shar

tgz : clobber
	cd ..; $(GNUTAR) -czvf reverse.tgz reverse

clean :
	-$(RM) *.o

wipe :
	-$(RM) *.o reverse reverse.man

clobber :
	-$(RM) *.o reverse reverse.man reverse.1
