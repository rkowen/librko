#ifndef _ANSI_SEQ_H_
#  define _ANSI_SEQ_H_
/* 
 * RCSID @(#)$Id: ansi_seq.h,v 1.2 2002/06/27 20:32:02 rk Exp $
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2001 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/
#  ifdef __cplusplus
extern "C" {
#  endif

/* ANSI_SEQ definitions */

/* define this if there is no isatty() call available */
#  if 0
#    define NO_ISATTY
#  endif


#  define ATTRIB_NUM	6
typedef enum {
   NONE,   BOLD,   UNDERSCORE,   BLINK,   REVERSE,   CONCEALED}
	ansi_attributes;

#  define COLOR_NUM	8
typedef enum {
   BLACK,   RED,   GREEN,   YELLOW,   BLUE,   MAGENTA,   CYAN,   WHITE}
	ansi_colors;

extern const char *ANSI_Colors[COLOR_NUM];
extern const char *ANSI_Attributes[ATTRIB_NUM];

#  define ANSISEQ_LEN	16
#  define ANSISEQ_NUM	32

int set_ansi_tty(int ans);
const char *ansi_seq(ansi_attributes a, ansi_colors b, ansi_colors f);
const char *ansi_color(ansi_colors b, ansi_colors f);
const char *ansi_fgcolor(ansi_colors f);
const char *ansi_bgcolor(ansi_colors b);
const char *ansi_attribute(ansi_attributes a);

#  ifdef __cplusplus
	}
#  endif
#endif /* _ANSI_SEQ_H_ */
