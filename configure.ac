AC_INIT([X Certificate and Key management],
	m4_esyscmd([tr -d '\n' < VERSION]),
	[christian@hohnstaedt.de],
	[xca],
	[http://xca.hohnstaedt.de])
AC_MSG_NOTICE([     ***************************************************])
AC_MSG_NOTICE([     *   ${PACKAGE_NAME} ${PACKAGE_VERSION}])
AC_MSG_NOTICE([     ***************************************************])
AC_PROG_CXX
AC_LANG(C++)
AC_CHECK_TOOL(STRIP, [strip], [:])

DVERSION="`${CXX} --version | grep apple-darwin || :`"
case "$DVERSION" in
  *-apple-darwin14*) DARWIN="Yosemite" ;;
  *-apple-darwin13*) DARWIN="Mavericks" ;;
  *-apple-darwin12*) DARWIN="Mountain Lion" ;;
  *-apple-darwin11*) DARWIN="Lion" ;;
  *-apple-darwin10*) DARWIN="SnowLeopard" ;;
  *-apple-darwin9*) DARWIN="Leopard" ;;
  *-apple-darwin?)
    AC_MSG_ERROR([echo Unsupported Mac OSX version $DVERSION])
    ;;
esac

if test -n "${DARWIN}"; then
  AC_MSG_NOTICE([Mac OSX ${DARWIN} detected])
  CXXFLAGS="${CXXFLAGS} -pipe -gdwarf-2"
fi
AC_SUBST([DARWIN])
AC_SUBST([MACDEPLOYQT])

export PKG_CONFIG_PATH
export LD_LIBRARY_PATH
if test -n "$DARWIN"; then
  export DYLD_LIBRARY_PATH
fi

if test "$srcdir" != "."; then
  if test -r "$srcdir"/Local.mak || test -r "$srcdir"/local.h; then
    AC_ERROR([The source directory (${srcdir}) contains a 'Local.mak' or 'local.h' file.
		They must be removed before building here.])
  fi
fi

# Detect the OpenSSL libraries and header
#########################################
AC_ARG_WITH([openssl],
  AS_HELP_STRING([--with-openssl], [Select the OpenSSL installation directory]),
  [ if test -d "$withval"; then
      _OPENSSLDIR="$withval"
    else
      AC_MSG_WARN([OpenSSL directory '$withval' does not exist or is not a directory])
    fi
  ], [_OPENSSLDIR="$OPENSSLDIR"])

if test -n "${_OPENSSLDIR}" && test -d "${_OPENSSLDIR}"; then
  _OPENSSLDIR=`cd ${_OPENSSLDIR} && pwd`
  PKG_CONFIG_PATH="${_OPENSSLDIR}/lib/pkgconfig:${PKG_CONFIG_PATH}"
  LD_LIBRARY_PATH="${_OPENSSLDIR}/lib:${LD_LIBRARY_PATH}"
  DYLD_LIBRARY_PATH="${_OPENSSLDIR}/lib:${DYLD_LIBRARY_PATH}"
fi

PKG_CHECK_MODULES([OPENSSL],
   [libcrypto >= 0.9.8], [ ],
   [
     OPENSSL_LIBS=" -lcrypto ";
     AC_MSG_WARN([OpenSSL pkg-config failed, using fallback defaults (${OPENSSL_LIBS})]);
   ]
)


# Detect the Qt libraries and header
####################################
AC_ARG_WITH([qt],
  AS_HELP_STRING([--with-qt], [Select the Qt installation directory]),
  [ if test -d "$withval"; then
      _QTDIR="$withval"
    else
       AC_MSG_WARN([Qt directory '$withval' does not exist or is not a directory])
    fi
  ], [_QTDIR="${QTDIR}"])

if test -n "${_QTDIR}" && test -d "${_QTDIR}"; then
  _QTDIR=`cd ${_QTDIR} && pwd`
  PKG_CONFIG_PATH="${_QTDIR}/lib/pkgconfig:${PKG_CONFIG_PATH}"
  DYLD_LIBRARY_PATH="${_QTDIR}/lib:${DYLD_LIBRARY_PATH}"
fi

PKG_CHECK_MODULES(QT, [QtCore >= 4.6, QtCore < 5.0, QtGui >= 4.6, QtGui < 5.0], [
  QT_MOC="`pkg-config --variable=moc_location QtCore`"
  QT_UIC="`pkg-config --variable=uic_location QtCore`"
  if test -n "$DARWIN"; then
    QT_CFLAGS="$QT_CFLAGS -F`pkg-config --variable=libdir QtCore`"
  fi
], [
  if test -z "$DARWIN"; then
    QT_LIBS=" -lQtCore -lQtGui "
  else
    QT_LIBS=" -framework QtGui -framework QtCore "
  fi
  AC_MSG_WARN([Qt pkg-config failed, using fallback defaults (${QT_LIBS})]);
])

# Setup MOC UIC RCC LRELEASE with absolute PATH
if test "x${QT_MOC}" = "x"; then
  QT_MOC="`which moc-qt4 || which moc`"
fi
if test "x${QT_UIC}" = "x"; then
  QT_UIC="`which uic-qt4 || which uic`"
fi
QT_LRELEASE=`echo ${QT_MOC} | sed 's/moc/lrelease/'`
if test -n "$DARWIN"; then
  MACDEPLOYQT=`echo ${QT_MOC} | sed 's/moc/macdeployqt/'`
fi
QT_RCC=`echo ${QT_MOC} | sed 's/moc/rcc/'`
if ! test -x ${QT_RCC}; then
  QT_RCC=`echo ${QT_MOC} | sed 's/moc.*/rcc/'`
fi
if ! test -x ${QT_RCC}; then
  QT_RCC="`which rcc-qt4 || which rcc`"
fi

AC_SUBST([QT_MOC])
AC_SUBST([QT_UIC])
AC_SUBST([QT_LRELEASE])
AC_SUBST([QT_RCC])

# The dyn_loader library libltdl
##################################
AC_CHECK_LIB(ltdl, lt_dlopen, , [
  echo "ERROR: Library 'ltdl' with symbol 'lt_dlopen' not found."
  echo "       Try installing the package 'libltdl-dev' or 'libtool'"
  exit 1
])
AC_CHECK_HEADER(ltdl.h, , [
  echo "ERROR: Header 'ltdl.h' not found."
  echo "       Try installing the package 'libltdl-dev' or 'libtool'"
  exit 1
])

# Just give it a try .....
##########################
XCA_COMPILE_TEST()

AX_CHECK_GNU_MAKE()

# linuxdoc application detection
##################################
AC_CHECK_PROGS([DOCTOOL], [linuxdoc sgml2html], [":"])
AC_SUBST([DOCTOOL])

case "$DOCTOOL" in
 linuxdoc) DOCTOOL="linuxdoc -B html";;
 sgml2html) DOCTOOL="sgml2html -s 1";;
 :)
  if test "${srcdir}" != "."; then
    mkdir -p doc
    cp ${srcdir}/doc/xca-doc.tgz doc/
  fi
  AC_MSG_WARN(['linuxdoc' or 'sgml2html' not found, using pre-compiled documentation])
  ;;
esac

# Setup done. Write local.h and Local.mak
############################################
AC_CONFIG_HEADERS(local.h)
AC_CONFIG_LINKS(Makefile:Makefile)
AC_OUTPUT([Local.mak])
