/* Read block from given position in file without changing file pointer.
   Syllable version.
   Copyright (C) 1997, 1998, 1999, 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

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

#include <errno.h>
#include <unistd.h>
#include <syllable/filesystem.h>

ssize_t
__libc_pread (int fd, void *buf, size_t nbyte, off_t offset)
{
  return read_pos (fd, offset, buf, nbyte);
}

#ifndef __libc_pread
strong_alias (__libc_pread, __pread)
weak_alias (__libc_pread, pread)
#endif
