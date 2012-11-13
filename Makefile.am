TESTS=
check_PROGRAMS=			${TESTS}
lib_LTLIBRARIES=		libserialdata.la
serialdatadir=			${includedir}/serialdata
serialdata_HEADERS=		serialdatawriter.h serialdatareader.h

libserialdata_la_SOURCES=	blockhead.pb.cc serialdatareader.cc	\
				serialdatawriter.cc
libserialdata_la_LDFLAGS=	-version-info ${LIBRARY_VERSION}
libserialdata_la_LIBADD=	${AC_LIBS}

CLEANFILES=	blockhead.pb.cc blockhead.pb.h

blockhead.pb.cc blockhead.pb.h: blockhead.proto
	protoc --cpp_out=. $<
