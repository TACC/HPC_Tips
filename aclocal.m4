dnl AX_PYTHON_MODULE(path_to_python, modname [, fatal ])

AC_DEFUN([AX_PYTHON_MODULE],[
	AC_MSG_CHECKING($1 module: $2)
	$1 -c "import $2" > /dev/null 2>&1
	if test $? -eq 0;
	then
		AC_MSG_RESULT(yes)
		eval AS_TR_CPP(HAVE_PYMOD_$2)=yes
	else
		AC_MSG_RESULT(no)
		eval AS_TR_CPP(HAVE_PYMOD_$2)=no
		#
		if test -n "$3"
		then
			AC_MSG_ERROR(failed to find required python module $2)
			exit 1
		fi
	fi
])


AC_DEFUN([AX_SUMMARIZE_CONFIG],
[
echo
echo '----------------------------------- SUMMARY ----------------------------------'
echo
echo "Prefix..............................................." : $prefix
echo "TIPS_HOST............................................" : $TIPS_HOST
echo "TIPS_READER.........................................." : $TIPS_READER
echo "TIPS_PASS............................................" : $TIPS_PASS
echo "TIPS_PASS64.........................................." : $TIPS_PASS64
echo "TIPS_DB.............................................." : $TIPS_DB
echo "TIPS_MODULE_NAME....................................." : $TIPS_MODULE_NAME
echo
echo '------------------------------------------------------------------------------'
echo

])
