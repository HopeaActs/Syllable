/* Copyright (C) 1991, 92, 93, 94, 96, 97 Free Software Foundation, Inc.
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

#include <stdio.h>
#include <hurd.h>

/* Rename the file OLD to NEW.  */
int
rename (old, new)
     const char *old;
     const char *new;
{
  error_t err;
  file_t olddir, newdir;
  const char *oldname, *newname;

  olddir = __file_name_split (old, (char **) &oldname);
  if (olddir == MACH_PORT_NULL)
    return -1;
  newdir = __file_name_split (new, (char **) &newname);
  if (newdir == MACH_PORT_NULL)
    {
       __mach_port_deallocate (__mach_task_self (), olddir);
      return -1;
    }

  err = __dir_rename (olddir, oldname, newdir, newname, 0);
  __mach_port_deallocate (__mach_task_self (), olddir);
  __mach_port_deallocate (__mach_task_self (), newdir);
  if (err)
    return __hurd_fail (err);
  return 0;
}
