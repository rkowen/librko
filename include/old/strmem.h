/* strmem header - allocates/deallocates collected string pool memory.
 *
 * author	R.K.Owen,Ph.D.	10/20/94
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
 *                   rkowen@ckns.net
 *                   smbd89a@prodigy.com
 *
 *********************************************************************
 */

#ifndef _STRMEM_H_
#  define _STRMEM_H_

void strmemalloc(char **ChArSpAcE, char ***charspace,
	size_t numstr, size_t maxstrlen);
void strmemfree(char **ChArSpAcE, char ***charspace);

#endif /* _STRMEM_H_ */
