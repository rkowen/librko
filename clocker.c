static const char RCSID[]="@(#)$Id: clocker.c,v 1.5 2002/09/13 01:50:44 rk Exp $";
static const char AUTHOR[]="@(#)clocker 1.0 2002/02/08 R.K.Owen,Ph.D.";
/* clocker - interfaces with the POSIX times() routine, since I found that
 *	the ANSI/Std C clock() doesn't always work.
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2002 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

#if defined(_AIX)
#  define _POSIX_SOURCE
#  include <time.h>		/* CLK_TCK */
#endif
#include <sys/times.h>		/* times */
#include <unistd.h>		/* sysconf */

#include "clocker.h"
#ifdef RKOERROR
#  include "rkoerror.h"
#endif
/* clocker - looks at the user time
 *	_SET	= sets the timer returns 0.0 if no error
 *	_RESET	= returns the current time and resets the timer
 *	_READ	= returns the current time and continues on
 *	_PER_SEC= returns the number of ticks per sec
 */
clocker_t clocker_tick(clocker_t *clock_variable, clocker_action what_to_do) {
	struct tms ts;
	clock_t tc = *clock_variable;
	
	switch(what_to_do) {
	case _SET:
		(void) times(&ts);
		*clock_variable = ts.tms_utime;
		tc = 0;
		break;
	case _RESET:
		(void) times(&ts);
		tc = ts.tms_utime - tc;
		*clock_variable = ts.tms_utime;
		break;
	case _READ:
		(void) times(&ts);
		tc = ts.tms_utime - tc;
		break;
	case _PER_SEC:
		tc = sysconf(_SC_CLK_TCK);
		break;
	default:
#ifdef RKOERROR
		(void) rkocpyerror("clocker_tick : should not 'get here'");
		return (clock_t) -1;
#endif
		break;
	}

	return tc;
}

double clocker(clocker_t *clock_variable, clocker_action what_to_do) {
	clock_t t;
	double x;

	t = clocker_tick(clock_variable, what_to_do);
	if (what_to_do == _PER_SEC) {
		x = (double) t;
	} else {
		x = (double) t / (double) sysconf(_SC_CLK_TCK);
	}
	return x;
}
