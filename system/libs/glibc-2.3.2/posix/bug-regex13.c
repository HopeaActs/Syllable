/* Regular expression tests.
   Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Isamu Hasegawa <isamu@yamato.ibm.com>, 2002.

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

#include <sys/types.h>
#include <mcheck.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct
{
  int syntax;
  const char *pattern;
  const char *string;
  int start;
} tests[] = {
  {RE_BACKSLASH_ESCAPE_IN_LISTS, "[0\\-9]", "1", -1}, /* It should not match.  */
  {RE_BACKSLASH_ESCAPE_IN_LISTS, "[0\\-9]", "-", 0}, /* It should match.  */
  {RE_SYNTAX_POSIX_BASIC, "s1\n.*\ns3", "s1\ns2\ns3", 0}
};

int
main (void)
{
  struct re_pattern_buffer regbuf;
  const char *err;
  size_t i;
  int ret = 0;

  mtrace ();

  for (i = 0; i < sizeof (tests) / sizeof (tests[0]); ++i)
    {
      int start;
      re_set_syntax (tests[i].syntax);
      memset (&regbuf, '\0', sizeof (regbuf));
      err = re_compile_pattern (tests[i].pattern, strlen (tests[i].pattern),
                                &regbuf);
      if (err != NULL)
	{
	  printf ("re_compile_pattern failed: %s\n", err);
	  ret = 1;
	  continue;
	}

      start = re_search (&regbuf, tests[i].string, strlen (tests[i].string),
                         0, strlen (tests[i].string), NULL);
      if (start != tests[i].start)
	{
	  printf ("re_search failed %d\n", start);
	  ret = 1;
	  regfree (&regbuf);
	  continue;
	}
      regfree (&regbuf);
    }

  return ret;
}
