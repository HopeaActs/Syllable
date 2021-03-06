/* Copyright (C) 1991, 1996, 1997, 1998, 1999 Free Software Foundation, Inc.
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct ltest
  {
    const char *str;		/* Convert this.  */
    double expect;		/* To get this.  */
    char left;			/* With this left over.  */
    int err;			/* And this in errno.  */
  };
static const struct ltest tests[] =
  {
    { "12.345", 12.345, '\0', 0 },
    { "12.345e19", 12.345e19, '\0', 0 },
    { "-.1e+9", -.1e+9, '\0', 0 },
    { ".125", .125, '\0', 0 },
    { "1e20", 1e20, '\0', 0 },
    { "0e-19", 0, '\0', 0 },
    { "4\00012", 4.0, '\0', 0 },
    { "5.9e-76", 5.9e-76, '\0', 0 },
    { NULL, 0, '\0', 0 }
  };

static void expand (char *dst, int c);
static int long_dbl (void);

int
main (int argc, char ** argv)
{
  char buf[100];
  register const struct ltest *lt;
  char *ep;
  int status = 0;
  int save_errno;

  for (lt = tests; lt->str != NULL; ++lt)
    {
      double d;

      errno = 0;
      d = strtod(lt->str, &ep);
      save_errno = errno;
      printf ("strtod (\"%s\") test %u",
	     lt->str, (unsigned int) (lt - tests));
      if (d == lt->expect && *ep == lt->left && save_errno == lt->err)
	puts ("\tOK");
      else
	{
	  puts ("\tBAD");
	  if (d != lt->expect)
	    printf ("  returns %.60g, expected %.60g\n", d, lt->expect);
	  if (lt->left != *ep)
	    {
	      char exp1[5], exp2[5];
	      expand (exp1, *ep);
	      expand (exp2, lt->left);
	      printf ("  leaves '%s', expected '%s'\n", exp1, exp2);
	    }
	  if (save_errno != lt->err)
	    printf ("  errno %d (%s)  instead of %d (%s)\n",
		    save_errno, strerror (save_errno),
		    lt->err, strerror (lt->err));
	  status = 1;
	}
    }

  sprintf (buf, "%f", strtod ("-0.0", NULL));
  if (strcmp (buf, "-0.000000") != 0)
    {
      printf ("  strtod (\"-0.0\", NULL) returns \"%s\"\n", buf);
      status = 1;
    }

  status |= long_dbl ();

  exit (status ? EXIT_FAILURE : EXIT_SUCCESS);
}

static void
expand (dst, c)
     char *dst;
     register int c;
{
  if (isprint (c))
    {
      dst[0] = c;
      dst[1] = '\0';
    }
  else
    (void) sprintf (dst, "%#.3o", (unsigned int) c);
}

static int
long_dbl (void)
{
  /* Regenerate this string using

     echo '(2^53-1)*2^(1024-53)' | bc | sed 's/\([^\]*\)\\*$/    "\1"/'

  */
  static const char longestdbl[] =
    "17976931348623157081452742373170435679807056752584499659891747680315"
    "72607800285387605895586327668781715404589535143824642343213268894641"
    "82768467546703537516986049910576551282076245490090389328944075868508"
    "45513394230458323690322294816580855933212334827479782620414472316873"
    "8177180919299881250404026184124858368";
  double d = strtod (longestdbl, NULL);

  printf ("strtod (\"%s\", NULL) = %g\n", longestdbl, d);

  if (d != 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000)
    return 1;

  return 0;
}
