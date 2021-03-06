/* Machine-specific calling sequence for `mcount' profiling function.  SuperH
   Copyright (C) 2001 Free Software Foundation, Inc.
   Contributed by NIIBE Yutaka <gniibe@m17n.org>
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <sysdep.h>

	ASM_GLOBAL_DIRECTIVE C_SYMBOL_NAME(_mcount)
	ASM_TYPE_DIRECTIVE (C_SYMBOL_NAME(_mcount),function)
	.align	5
C_LABEL(_mcount)
	/* Save registers.  */
	mov.l	r4,@-r15
	mov.l	r5,@-r15
	mov.l	r6,@-r15
	mov.l	r7,@-r15
	sts.l	pr,@-r15

	mov.l	@(20,r15),r4
	sts	pr,r5

#ifdef SHARED
	mov.l	0f,r1
	mova	0f,r0
	add	r1,r0
	mov.l	1f,r1
	mov.l	@(r0,r1),r1
#else
	mov.l	1f,r1
#endif
	jsr	@r1
	 nop

	/* Pop the saved registers.  */
	lds.l	@r15+,pr
	mov.l	@r15+,r7
	mov.l	@r15+,r6
	mov.l	@r15+,r5
	rts
	 mov.l	@r15+,r4

	.align 2
#ifdef SHARED
0:	.long	_GLOBAL_OFFSET_TABLE_
1:	.long	C_SYMBOL_NAME(__mcount_internal)@GOT
#else
1:	.long	C_SYMBOL_NAME(__mcount_internal)
#endif
ASM_SIZE_DIRECTIVE(C_SYMBOL_NAME(_mcount))

#undef mcount
weak_alias (_mcount, mcount)
