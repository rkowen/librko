static const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: timedfgets.c,v 1.3 1998/10/23 20:20:55 rk Exp $";
static const char AUTHOR[]="@(#)timedfgets 1.0 03/26/96 R.K.Owen,Ph.D.";
/* timedfgets  -  performs an fgets and times out if input is
 * not received by the specified amount of time.
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1997
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: rkowen@ckns.net
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */
 
#define _POSIX_SOURCE

#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#ifdef RKOERROR
#  include "librko.h"
#endif
#ifndef SA_RESTART
#  define NO_SA_RESTART
#endif

static jmp_buf TFGSjumpbuffer;		/* longjump global variable */

static void TFGSsigalrm_handler(int signum) {
	/* alarm went off - the timedfgets has timed out - jump back */
	longjmp(TFGSjumpbuffer, 1);
}

int timedfgets(char *buf, int size, FILE *stream, int seconds) {

	struct sigaction newsigalrm, oldsigalrm;/* ALRM interrupt */
	int jumpval;				/* longjump return value */
	unsigned old_alarm_time, new_alarm_time;/* alarm times */
	int returnstatus = (int) NULL;		/* this function status */

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	/* set-up interrupt for SIGALRM */
	newsigalrm.sa_handler = TFGSsigalrm_handler;
	sigemptyset(&newsigalrm.sa_mask);
#ifndef NO_SA_RESTART
	newsigalrm.sa_flags = SA_RESTART;
#endif

	if (sigaction(SIGALRM, &newsigalrm, &oldsigalrm) < 0) {
#ifdef RKOERROR
		rkoerrno = RKOSIGERR;
		rkocpyerror("timedfgets : signal set error");
#endif
		return returnstatus;
	}

	/* set place for longjump */
	jumpval = setjmp(TFGSjumpbuffer);

	/* is this the first time through, or did we longjump here? */
	switch (jumpval) {
	case 0:			/* 1st time through - set up alarm */
		if (seconds < 0) {
#ifdef RKOERROR
			rkocpyerror("timedfgets : user alarm time error");
			rkoerrno = RKOUSEERR;
#endif
			return returnstatus;
		} else {
			new_alarm_time = (unsigned) seconds;
		}
		/* don't interfere with alarm if time-out = 0 seconds */
		if (seconds) old_alarm_time = alarm(new_alarm_time);
		/* read line of input ... */
		(void) fgets(buf, size, stream);

		/* check for EOF */
		if (feof(stream)) {
			returnstatus = -1;
			errno = EIO;
		}
		break;
	case 1:			/* alarm went off */
		returnstatus = -1;
		errno = EINTR;
		break;
	default:		/* should not get here */
#ifdef RKOERROR
		rkoerrno = RKOGENERR;
		rkocpyerror("timedfgets : `should not get here' error");
#endif
		return returnstatus;
	}

	/* guard against another SIGALRM while resetting handler */
	(void) alarm(0);

	/* reset SIGALRM before leaving */
	if (sigaction(SIGALRM, &oldsigalrm, NULL) < 0) {
#ifdef RKOERROR
		rkoerrno = RKOSIGERR;
		rkocpyerror("timedfgets : signal reset error");
#endif
		return returnstatus;
	}
	/* reset alarm before leaving */
	(void) alarm(old_alarm_time);

	return returnstatus;
}
