/* Cancel requests associated with given file descriptor.
   Copyright (C) 1997, 1998 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

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


/* We use an UGLY hack to prevent gcc from finding us cheating.  The
   implementation of aio_cancel and aio_cancel64 are identical and so
   we want to avoid code duplication by using aliases.  But gcc sees
   the different parameter lists and prints a warning.  We define here
   a function so that aio_cancel64 has no prototype.  */
#define aio_cancel64 XXX
#include <aio.h>
/* And undo the hack.  */
#undef aio_cancel64

#include <errno.h>

#include "aio_misc.h"


int
aio_cancel (fildes, aiocbp)
     int fildes;
     struct aiocb *aiocbp;
{
  struct requestlist *req = NULL;
  int result = AIO_ALLDONE;

  /* Request the mutex.  */
  pthread_mutex_lock (&__aio_requests_mutex);

  /* We are asked to cancel a specific AIO request.  */
  if (aiocbp != NULL)
    {
      /* If the AIO request is not for this descriptor it has no value
	 to look for the request block.  */
      if (aiocbp->aio_fildes == fildes)
	{
	  struct requestlist *last = NULL;

	  req = __aio_find_req_fd (fildes);

	  while (req->aiocbp != (aiocb_union *) aiocbp)
	    {
	      last = req;
	      req = req->next_prio;
	    }

	  /* Don't remove the entry if a thread is already working on it.  */
	  if (req->running == allocated)
	    result = AIO_NOTCANCELED;
	  else
	    {
	      /* We can remove the entry.  */
	      if (last != NULL)
		last->next_prio = req->next_prio;
	      else
		if (req->next_prio == NULL)
		  {
		    if (req->last_fd != NULL)
		      req->last_fd->next_fd = req->next_fd;
		    if (req->next_fd != NULL)
		      req->next_fd->last_fd = req->last_fd;
		  }
		else
		  {
		    if (req->last_fd != NULL)
		      req->last_fd->next_fd = req->next_prio;
		    if (req->next_fd != NULL)
		      req->next_fd->last_fd = req->next_prio;
		    req->next_prio->last_fd = req->last_fd;
		    req->next_prio->next_fd = req->next_fd;

		    /* Mark this entry as runnable.  */
		    req->next_prio->running = yes;
		  }

	      result = AIO_CANCELED;
	    }

	  req->next_prio = NULL;
	}
    }
  else
    {
      /* Find the beginning of the list of all requests for this
	 desriptor.  */
      req = __aio_find_req_fd (fildes);

      /* If any request is worked on by a thread it must be the first.
	 So either we can delete all requests or all but the first.  */
      if (req != NULL)
	{
	  if (req->running == allocated)
	    {
	      struct requestlist *old = req;
	      req = req->next_prio;
	      old->next_prio = NULL;

	      result = AIO_NOTCANCELED;
	    }
	  else
	    {
	      /* Remove entry from the file descriptor list.  */
	      if (req->last_fd != NULL)
		req->last_fd->next_fd = req->next_fd;
	      if (req->next_fd != NULL)
		req->next_fd->last_fd = req->last_fd;

	      result = AIO_CANCELED;
	    }
	}
    }

  /* Mark requests as canceled and send signal.  */
  while (req != NULL)
    {
      struct requestlist *old = req;
      req->aiocbp->aiocb.__error_code = ECANCELED;
      req->aiocbp->aiocb.__return_value = -1;
      __aio_notify (req);
      req = req->next_prio;
      __aio_free_request (old);
    }

  /* Release the mutex.  */
  pthread_mutex_unlock (&__aio_requests_mutex);

  return result;
}

weak_alias (aio_cancel, aio_cancel64)
