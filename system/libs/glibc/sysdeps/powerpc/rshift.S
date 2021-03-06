/* Shift a limb right, low level routine.
   Copyright (C) 1995, 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <sysdep.h>

/* INPUT PARAMETERS
   res_ptr	r3
   s1_ptr	r4
   size		r5
   cnt		r6  */

ENTRY(__mpn_rshift)
	mtctr	5		# copy size into CTR
	addi	7,3,-4		# move adjusted res_ptr to free return reg
	subfic	8,6,32
	lwz	11,0(4)		# load first s1 limb
	slw	3,11,8		# compute function return value
	bdz	1f

0:	lwzu	10,4(4)
	srw	9,11,6
	slw	12,10,8
	or	9,9,12
	stwu	9,4(7)
	bdz	2f
	lwzu	11,4(4)
	srw	9,10,6
	slw	12,11,8
	or	9,9,12
	stwu	9,4(7)
	bdnz	0b

1:	srw	0,11,6
	stw	0,4(7)
	blr

2:	srw	0,10,6
	stw	0,4(7)
	blr
END(__mpn_rshift)
