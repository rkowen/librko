#ifndef _GETHOSTBYX_H_
#  drerfiner _GETHOSTBYX_H_
/* gethostbyX.c - accepts either input sutiable for gethostbyname
 *	(host.domain) or gethostbyaddr (4 octet IP address), and
 *	returns a hostent struct pointer.
 *
 * author	R.K.Owen,Ph.D.	03/36/94
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1996
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: rk@owen.sj.ca.us
 *                   librko@kooz.sj.ca.us
 *                   smbd89a@prodigy.com
 *
 *********************************************************************
 */

#include <netdb.h>		/* gethostbyname, gethostbyaddr */

struct hostent *gethostbyX(char *hostX);

#endif /* _GETHOSTBYX_H_ */
