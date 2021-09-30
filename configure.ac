AC_INIT(Makefile.in)
AC_CONFIG_HEADERS(src/__build__/header.h:src/header.h.in)
AC_PROG_CC
AC_PREFIX_DEFAULT(/usr/local)

AH_TOP(
[
/* Remember that AC_DEFINE() end up in header.h.in
 * AC_SUBST() end up in config.h.in
 */
])

AC_SUBST(TIPS_HOST)
AC_ARG_WITH(host,
    AC_HELP_STRING([--with-host=ans],[The hostname for where the database resides]),
    TIPS_HOST="$withval"
    AC_MSG_RESULT([TIPS_HOST=$with_host])
    AC_DEFINE_UNQUOTED(TIPS_HOST, "$with_host",[The hostname for where the database resides])dnl
    ,
    withval="<unknown>"
    AC_MSG_RESULT([TIPS_HOST=$withval])
    TIPS_HOST="$withval"
    AC_DEFINE_UNQUOTED(TIPS_HOST, "$withval",[The hostname for where the database resides]))dnl

AC_SUBST(MYSQLDB)
AC_ARG_WITH(MySQL,
    AC_HELP_STRING([--with-MySQL=ans],[Require the mysqlclient/MySQL-python module [[yes]]]),
    MYSQLDB="$withval"
    MYSQLDB=`echo $MYSQLDB | tr '@<:@:upper:@:>@' '@<:@:lower:@:>@'`
    VALID_YN $MYSQLDB "--with-MySQL=ans: ans must be yes or no"
    AC_MSG_RESULT([MYSQLDB=$with_MySQL])
    AC_DEFINE_UNQUOTED(MYSQLDB, "$with_MySQL",[Require the mysqlclient/MySQL-python module])dnl
    ,
    withval="yes"
    AC_MSG_RESULT([MYSQLDB=$withval])
    MYSQLDB="$withval"
    AC_DEFINE_UNQUOTED(MYSQLDB, "$withval",[Require the mysqlclient/MySQL-python module]))dnl

AC_SUBST(TIPS_READER)
AC_ARG_WITH(reader,
    AC_HELP_STRING([--with-reader=ans],[The username for the tips database reader[[readerOfTips]]]),
    TIPS_READER="$withval"
    AC_MSG_RESULT([TIPS_READER=$with_reader])
    AC_DEFINE_UNQUOTED(TIPS_READER, "$with_reader",[The username for the tips database reader])dnl
    ,
    withval="readerOfTips"
    AC_MSG_RESULT([TIPS_READER=$withval])
    TIPS_READER="$withval"
    AC_DEFINE_UNQUOTED(TIPS_READER, "$withval",[The username for the tips database reader]))dnl

AC_SUBST(TIPS_PASS)
AC_ARG_WITH(pass,
    AC_HELP_STRING([--with-pass=ans],[The password for the tips database reader]),
    TIPS_PASS="$withval"
    AC_MSG_RESULT([TIPS_PASS=$with_pass])
    AC_DEFINE_UNQUOTED(TIPS_PASS, "$with_pass",[The password for the tips database reader])dnl
    ,
    withval="<unknown>"
    AC_MSG_RESULT([TIPS_PASS=$withval])
    TIPS_PASS="$withval"
    AC_DEFINE_UNQUOTED(TIPS_PASS, "$withval",[The password for the tips database reader]))dnl

AC_SUBST(TIPS_DB)
AC_ARG_WITH(db,
    AC_HELP_STRING([--with-db=ans],[The database name for the tips [[HPCTips]]]),
    TIPS_DB="$withval"
    AC_MSG_RESULT([TIPS_DB=$with_db])
    AC_DEFINE_UNQUOTED(TIPS_DB, "$with_db",[The database name for the tips])dnl
    ,
    withval="HPCTips"
    AC_MSG_RESULT([TIPS_DB=$withval])
    TIPS_DB="$withval"
    AC_DEFINE_UNQUOTED(TIPS_DB, "$withval",[The database name for the tips]))dnl

AC_SUBST(TIPS_MODULE_NAME)
AC_ARG_WITH(module,
    AC_HELP_STRING([--with-module=ans],[The module name for the tips[[HPC_Tips]]]),
    TIPS_MODULE_NAME="$withval"
    AC_MSG_RESULT([TIPS_MODULE_NAME=$with_module])
    AC_DEFINE_UNQUOTED(TIPS_MODULE_NAME, "$with_module",[The module name for the tips])dnl
    ,
    withval="HPC_Tips"
    AC_MSG_RESULT([TIPS_MODULE_NAME=$withval])
    TIPS_MODULE_NAME="$withval"
    AC_DEFINE_UNQUOTED(TIPS_MODULE_NAME, "$withval",[The module name for the tips]))dnl


if test "$TIPS_HOST" = "<unknown>"; then
   echo
   echo "==================================================================="
   echo "Unable to install hpc_tips without the hostname for the HPC_Tips DB"
   echo "README.md details"
   echo "==================================================================="
   rm -f makefile
   exit 1
fi

if test "$TIPS_PASS" = "<unknown>"; then
   echo
   echo "====================================================================="
   echo "Unable to install hpc_tips without the passwd for the HPC_Tips reader"
   echo "README.md details"
   echo "====================================================================="
   rm -f makefile
   exit 1
fi


AC_SUBST(TIPS_PASS64)
TIPS_PASS64=`echo $TIPS_PASS | base64 -`
AC_DEFINE_UNQUOTED(TIPS_PASS64, "$TIPS_PASS64",[base64 encoded reader password])

AC_SUBST(HAVE_PYMOD_MYSQLDB)
AX_PYTHON_MODULE([$PATH_TO_PYTHON],[MySQLdb])

if test "$MYSQLDB" = yes; then
   AC_PATH_PROGS(PATH_TO_PYTHON, [python3] [python2] [python], [])
   if test "x$PATH_TO_PYTHON" = "x"; then
      echo
      echo "======================================================="
      echo "HPC_Tips requires python3, python2 or python to run."
      echo "======================================================="
      echo
      rm -f makefile
      exit 1
   fi 

   if test "$HAVE_PYMOD_MYSQLDB" = no; then
      echo
      echo "======================================================="
      echo "Unable to install XALT without the python module: MySQLdb"
      echo "To install for python3 do: pip3 install mysqlclient"
      echo "To install for python2 do: pip  install MySQL-python"
      echo
      echo "MySQLdb is only required to be install on computer which"
      echo "will update the database.  It is not required anywhere"
      echo "else."
      echo "======================================================="
      rm -f makefile
      exit 1
   fi
fi

AC_CHECK_HEADER(mysql/my_global.h,
                [AC_DEFINE([HAVE_MYSQL_MY_GLOBAL_H], 1,
                   [Define to 1 if you have <mysql/my_global.h>.])],
                [])
AC_CHECK_HEADER(mysql/mysql.h,
                [AC_DEFINE([HAVE_MYSQL_MYSQL_H], 1,
                   [Define to 1 if you have <mysql/mysql.h>.])],
                [AC_MSG_ERROR([Unable to build HPC_Tips without mysql/mysql.h])])
                
AC_OUTPUT(makefile:Makefile.in src/__build__/config.h:src/config.h.in src/makefile:src/Makefile.in)

AX_SUMMARIZE_CONFIG
