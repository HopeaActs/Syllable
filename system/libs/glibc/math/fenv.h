/* Copyright (C) 1997 Free Software Foundation, Inc.
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

/*
 * ISO C 9X 7.6: Floating-point environment	<fenv.h>
 */

#ifndef _FENV_H
#define _FENV_H	1

#include <features.h>

/* Get the architecture dependend definitions.  The following definitions
   are expected to be done:

   fenv_t	type for object representing an entire floating-point
		environment

   FE_DFL_ENV	macro of type pointer to fenv_t to be used as the argument
		to functions taking an argument of type fenv_t; in this
		case the default environment will be used

   fexcept_t	type for object representing the floating-point exception
		flags including status associated with the flags

   The following macros are defined iff the implementation supports this
   kind of exception.
   FE_INEXACT		inexact result
   FE_DIVBYZERO		devision by zero
   FE_UNDERFLOW		result not representable due to underflow
   FE_OVERFLOW		result not representable due to overflow
   FE_INVALID		invalid operation

   FE_ALL_EXCEPT	bitwise OR of all supported exceptions

   The next macros are defined iff the appropriate rounding mode is
   supported by the implementation.
   FE_TONEAREST		round to nearest
   FE_UPWARD		round toward +Inf
   FE_DOWNWARD		round toward -Inf
   FE_TOWARDZERO	round toward 0
*/
#include <bits/fenv.h>

__BEGIN_DECLS

/* Floating-point exception handling.  */

/* Clear the supported exceptions represented by EXCEPTS.  */
extern void feclearexcept __P ((int __excepts));

/* Store implementation-defined representation of the exception flags
   indicated by EXCEPTS in the object pointed to by FLAGP.  */
extern void fegetexceptflag __P ((fexcept_t *__flagp, int __excepts));

/* Raise the supported exceptions represented by EXCEPTS.  */
extern void feraiseexcept __P ((int __excepts));

/* Set complete status for exceptions indicated by EXCEPTS according to
   the representation in the object pointed to by FLAGP.  */
extern void fesetexceptflag __P ((__const fexcept_t *__flagp, int __excepts));

/* Determine which of subset of the exceptions specified by EXCEPTS are
   currently set.  */
extern int fetestexcept __P ((int __excepts));


/* Rounding control.  */

/* Get current rounding direction.  */
extern int fegetround __P ((void));

/* Establish the rounding direction represented by ROUND.  */
extern int fesetround __P ((int __rounding_direction));


/* Floating-point environment.  */

/* Store the current floating-point environment in the object pointed
   to by ENVP.  */
extern void fegetenv __P ((fenv_t *__envp));

/* Save the current environment in the object pointed to by ENVP, clear
   exception flags and install a non-stop mode (if available) for all
   exceptions.  */
extern int feholdexcept __P ((fenv_t *__envp));

/* Establish the floating-point environment represented by the object
   pointed to by ENVP.  */
extern void fesetenv __P ((__const fenv_t *__envp));

/* Save current exceptions in temporary storage, install environment
   represented by object pointed to by ENVP and raise exceptions
   according to saved exceptions.  */
extern void feupdateenv __P ((__const fenv_t *__envp));

__END_DECLS

#endif /* fenv.h */
