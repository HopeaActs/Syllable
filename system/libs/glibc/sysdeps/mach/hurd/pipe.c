/* Copyright (C) 1992, 93, 94, 95, 96, 99 Free Software Foundation, Inc.
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

#include <errno.h>
#include <unistd.h>
#include <stddef.h>
#include <hurd.h>
#include <hurd/fd.h>
#include <sys/socket.h>
#include <hurd/socket.h>
#include <fcntl.h>

/* Create a one-way communication channel (pipe).
   If successful, two file descriptors are stored in FDS;
   bytes written on FDS[1] can be read from FDS[0].
   Returns 0 if successful, -1 if not.  */
int
__pipe (fds)
     int fds[2];
{
  error_t err;
  socket_t server, sock1, sock2;
  int d1, d2;

  if (fds == NULL)
    return __hurd_fail (EINVAL);

  /* Find the local domain socket server.  */
  server = _hurd_socket_server (PF_LOCAL, 0);
  if (server == MACH_PORT_NULL)
    return -1;

  /* Create two local domain sockets and connect them together.  */

  err = __socket_create (server, SOCK_STREAM, 0, &sock1);
  if (err == MACH_SEND_INVALID_DEST || err == MIG_SERVER_DIED
      || err == MIG_BAD_ID || err == EOPNOTSUPP)
    {
      /* On the first use of the socket server during the operation,
	 allow for the old server port dying.  */
      server = _hurd_socket_server (PF_LOCAL, 1);
      if (server == MACH_PORT_NULL)
	return -1;
      err = __socket_create (server, SOCK_STREAM, 0, &sock1);
    }
  if (err)
    return __hurd_fail (err);
  if (err = __socket_create (server, SOCK_STREAM, 0, &sock2))
    {
      __mach_port_deallocate (__mach_task_self (), sock1);
      return __hurd_fail (err);
    }
  if (err = __socket_connect2 (sock1, sock2))
    {
      __mach_port_deallocate (__mach_task_self (), sock1);
      __mach_port_deallocate (__mach_task_self (), sock2);
      return __hurd_fail (err);
    }

  /* Put the sockets into file descriptors.  */

  d1 = _hurd_intern_fd (sock1, O_IGNORE_CTTY, 1);
  if (d1 < 0)
    {
      __mach_port_deallocate (__mach_task_self (), sock2);
      return -1;
    }
  d2 = _hurd_intern_fd (sock2, O_IGNORE_CTTY, 1);
  if (d2 < 0)
    {
      err = errno;
      (void) close (d1);
      return __hurd_fail (err);
    }

  fds[0] = d1;
  fds[1] = d2;
  return 0;
}

weak_alias (__pipe, pipe)
