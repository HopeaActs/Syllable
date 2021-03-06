## Process this file with automake to produce Makefile.in -*-Makefile-*-.

TESTS = help-version
TESTS_ENVIRONMENT = \
  all_programs="$(bin_SCRIPTS) $(bin_PROGRAMS)"

EXTRA_DIST = Fetish.pm .env-warn envvar-check lang-default \
  expensive rwx-to-mode setgid-check group-names $(TESTS)
SUBDIRS = chgrp chmod cp dd du dircolors install ln ls ls-2 mkdir mv \
  rm rmdir shred touch

check-root:
	cd cp    && $(MAKE) check TESTS=special-bits
	cd touch && $(MAKE) check TESTS=fifo
	cd mv    && $(MAKE) check TESTS=mv-special-1

check-recursive: envvar-check root-hint

# Warn when `make check' is run with POSIXLY_CORRECT or CDPATH set.
envvar-check:
	test "$${POSIXLY_CORRECT+set}" = set \
	  && sed s/%%/POSIXLY_CORRECT/ $(srcdir)/.env-warn || :
	test "$${CDPATH+set}" = set \
	  && sed s/%%/CDPATH/ $(srcdir)/.env-warn || :

# Advertise `check-root' target.
root-hint:
	@echo '********************************************'
	@echo "NOTICE: Some tests will or might need to be"
	@echo "        run as root. Do \`make check-root'"
	@echo "        as \`root' to run just these tests."
	@echo '********************************************'
