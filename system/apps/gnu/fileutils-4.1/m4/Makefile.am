## Process this file with automake to produce Makefile.in -*-Makefile-*-

##m4-files-begin
EXTRA_DIST = README Makefile.am.in \
afs.m4 \
assert.m4 \
bison.m4 \
c-bs-a.m4 \
check-decl.m4 \
chown.m4 \
codeset.m4 \
d-ino.m4 \
d-type.m4 \
dos.m4 \
error.m4 \
fnmatch.m4 \
fpending.m4 \
fstypename.m4 \
fsusage.m4 \
ftruncate.m4 \
getgroups.m4 \
getline.m4 \
getloadavg.m4 \
glibc.m4 \
glibc21.m4 \
gnu-source.m4 \
group-member.m4 \
iconv.m4 \
inttypes_h.m4 \
isc-posix.m4 \
jm-glibc-io.m4 \
jm-macros.m4 \
jm-mktime.m4 \
jm-winsz1.m4 \
jm-winsz2.m4 \
lchown.m4 \
lcmessage.m4 \
lib-check.m4 \
libintl.m4 \
link-follow.m4 \
ls-mntd-fs.m4 \
lstat.m4 \
malloc.m4 \
mbrtowc.m4 \
mbstate_t.m4 \
mbswidth.m4 \
memcmp.m4 \
nanosleep.m4 \
perl.m4 \
prereq.m4 \
progtest.m4 \
putenv.m4 \
readdir.m4 \
realloc.m4 \
regex.m4 \
rename.m4 \
rmdir-errno.m4 \
search-libs.m4 \
st_dm_mode.m4 \
st_mtim.m4 \
stat.m4 \
strerror_r.m4 \
strftime.m4 \
timespec.m4 \
uintmax_t.m4 \
ulonglong.m4 \
unlink-busy.m4 \
uptime.m4 \
utimbuf.m4 \
utime.m4 \
utimes.m4 \
xstrtoumax.m4
##m4-files-end

Makefile.am: Makefile.am.in
	rm -f $@ $@t
	sed -n '1,/^##m4-files-begin/p' $< > $@t
	( (echo EXTRA_DIST = README Makefile.am.in | tr '\012' @); \
	  (echo *.m4|tr ' ' @) ) \
	  |sed 's/@$$/%/;s/@/ \\@/g' |tr @% '\012\012' \
	  >> $@t
	sed -n '/^##m4-files-end/,$$p' $< >> $@t
	chmod a-w $@t
	mv $@t $@
