/* quick & dirty program to create an ANSI color sequence
 * by R.K. Owen, Ph.D.  12/21/95
 */

#ifndef _ANSI_SEQ_H_
#  define _ANSI_SEQ_H_
/*
# Below are the color init strings for the basic file types. A color init
# string consists of one or more of the following numeric codes:
# Attribute codes: 
# 00=none 01=bold 04=underscore 05=blink 07=reverse 08=concealed
# Text color codes:
# 30=black 31=red 32=green 33=yellow 34=blue 35=magenta 36=cyan 37=white
# Background color codes:
# 40=black 41=red 42=green 43=yellow 44=blue 45=magenta 46=cyan 47=white

an ANSI color sequence is initiated with ^[[xx;xx;xxm  where xx are the
values specified above and ^[ refers to an <esc>
*/
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

#endif /* _ANSI_SEQ_H_ */
