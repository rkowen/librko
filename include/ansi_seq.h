#ifndef _ANSI_SEQ_H_
#  define _ANSI_SEQ_H_
/* 
 * RCSID @(#)$Id: ansi_seq.h,v 1.1 2002/02/15 23:01:55 rk Exp $
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 2001
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: librko@kooz.sj.ca.us
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */
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
