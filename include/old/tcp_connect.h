#ifndef _TCP_CONNECT_H_
#  define _TCP_CONNECT_H_
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
 * last known email: rkowen@kudonet.com
 *                   rk@owen.sj.ca.us
 *                   smbd89a@prodigy.com
 *
 *********************************************************************
 */

int tcp_connect(char *hostname, int port_num);
int tcp_close(int sd);

#endif /* _TCP_CONNECT_H_ */
