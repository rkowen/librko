# RCSID = @(#)$Id: Makefile,v 1.4 1999/09/09 21:31:08 rk Exp $  
#
# This Makefile will build a variety of test programs for each of the
#   modules.  After that you can just include the $METOBJ macro in
#   your own Makefile.
#
#********************************************************************
#
#     This software is copyrighted by R.K.Owen,Ph.D. 1997
#
# The author, R.K.Owen, of this software is not liable for any
# problems WHATSOEVER which may result from use  or  abuse  of
# this software. The author, R.K.Owen, grants unlimited rights
# to anyone who uses and modifies this  software  for  private
# non-commercial  use  as  long  as  this copyright and rights
# notice remains in this software and is made available to all
# recipients of this software.
#
# last known email: librko@kooz.sj.ca.us
#                   rk@owen.sj.ca.us
#
#********************************************************************
#
# The following macros might need to be defined for your configuration
#
#CC=gcc
LD=$(CC) $(LDFLAGS) -o
LIB=-L.. -lrko
#CFLAGS= -g
#
# 
RM	=rm
MV	=mv
CP	=cp
ECHO	=echo

# note that objects are already optimally ordered for one-pass loading
METOBJ= metropolis.o \
	ranindex.o ranswap.o ranperm.o \
	idxswap.o idxperm.o idxrevr.o

all : $(METOBJ)

help :
	@$(ECHO) "   Target:"
	@$(ECHO) "     all	-	makes the metropolis library & man page"
	@$(ECHO) "     tall	-	makes all the test programs"
	@$(ECHO) "     trun	-	run the test programs"
	@$(ECHO) "     tclean	-	removes all the test programs"
	@$(ECHO) "     clean	-	removes all the *.o files"
	@$(ECHO) "     wipe	-	removes most everything"
	@$(ECHO) "     clobber	-	removes almost everything"
	@$(ECHO) "     help	-	this info"

tall : tmet tranindex tidxrevr transwap tranperm tidxswap tidxperm

trun : 
	-./trun.csh

tmet : $(METOBJ)
	$(CC) $(CFLAGS) -c -DTEST metropolis.c
	$(LD) tmet $(METOBJ) -lm $(LIB)
	-$(RM) metropolis.o

tranindex : ranindex.o
	$(CC) $(CFLAGS) -c -DTEST ranindex.c
	$(LD) tranindex ranindex.o $(LIB)
	-$(RM) ranindex.o

tidxrevr : idxrevr.o ranswap.o
	$(CC) $(CFLAGS) -c -DTEST idxrevr.c
	$(LD) tidxrevr idxrevr.o ranswap.o $(LIB)
	-$(RM) idxrevr.o

transwap : ranswap.o
	$(CC) $(CFLAGS) -c -DTEST ranswap.c
	$(LD) transwap ranswap.o $(LIB)
	-$(RM) ranswap.o

tranperm : ranperm.o
	$(CC) $(CFLAGS) -c -DTEST ranperm.c
	$(LD) tranperm ranperm.o $(LIB)
	-$(RM) ranperm.o

tidxswap : idxswap.o
	$(CC) $(CFLAGS) -c -DTEST idxswap.c
	$(LD) tidxswap idxswap.o
	-$(RM) idxswap.o

tidxperm : idxperm.o
	$(CC) $(CFLAGS) -c -DTEST idxperm.c
	$(LD) tidxperm idxperm.o
	-$(RM) idxperm.o

#libmetropolis.a : ${METOBJ}
#	$(AR) rv libmetropolis.a ${METOBJ}

#metropolis.man : Metropolis.man
#	$(SED) \
#		-e "s:/LIBDIR/:${LIBDIR}/:" \
#		-e "s:/INCDIR/:${INCDIR}/:" \
#		Metropolis.man > metropolis.man
#
#metropolis.3 : metropolis.man
#	$(NROFF) $(MAN) metropolis.man > metropolis.3

tclean :
	-$(RM) tmet tranindex tidxrevr transwap tranperm tidxswap \
		tidxperm

clean :
	-$(RM) *.o

wipe : clean

clobber : wipe tclean
