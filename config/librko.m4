dnl ---------------------------------------------------------------------------
dnl special macros used by librko package
dnl ---------------------------------------------------------------------------
dnl LR_CHECK_PROGS(VARIABLE, programs, Path:to:search)
dnl VARIABLE is set to the first one found
AC_DEFUN([LR_CHECK_PROGS],
[for ac_prog in $2
do
	as_save_IFS=$IFS; IFS=$PATH_SEPARATOR
	for as_dir in $PATH
	do
		IFS=$as_save_IFS
		test -z "$as_dir" && as_dir=.
		if $as_executable_p "$as_dir/$ac_prog"; then
			AC_MSG_RESULT([$1 = $as_dir/$ac_prog])
			AC_DEFINE_UNQUOTED([$1],["$as_dir/$ac_prog"])
			break 2
		fi
	done
done
])dnl LR_CHECK_PROGS

