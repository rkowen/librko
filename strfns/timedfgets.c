static const char RCSID[]="@(#)$Id: timedfgets.c,v 1.9 2002/08/23 15:18:11 rk Exp $";
static const char AUTHOR[]="@(#)timedfgets 1.0 03/26/96 R.K.Owen,Ph.D.";
/* timedfgets  -  performs an fgets and times out if input is
 * not received by the specified amount of time.
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 1997 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/
 
#define _POSIX_SOURCE

#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>	/* alarm */
#include "config.h"
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

/* need to use the POSIX sigjmp_buf to save the current signal mask
 * else if you make repeated calls, then the second SIGALRM will
 * not be caught, and it will hang in the read()
 */
static sigjmp_buf TFGSjumpbuffer;	/* siglongjump global variable */

static void TFGSsigalrm_handler(int signum) {
	/* alarm went off - the timedfgets has timed out - jump back */
	siglongjmp(TFGSjumpbuffer, 1);
}

int timedfgets(char *buf, int size, FILE *stream, int seconds) {

	struct sigaction newsigalrm, oldsigalrm;/* ALRM interrupt */
	int jumpval = 0;			/* longjump return value */
	unsigned old_alarm_time, new_alarm_time;/* alarm times */
	int returnstatus = 0;			/* this function status */

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	/* set-up interrupt for SIGALRM */
	newsigalrm.sa_handler = TFGSsigalrm_handler;
	sigemptyset(&newsigalrm.sa_mask);
#ifdef SA_RESTART
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
	jumpval = sigsetjmp(TFGSjumpbuffer,1);

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
