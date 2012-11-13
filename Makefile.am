TESTS=				read_write_test
check_PROGRAMS=			${TESTS}
lib_LTLIBRARIES=		libserialdata.la
serialdatadir=			${includedir}/serialdata
serialdata_HEADERS=		serialdatawriter.h serialdatareader.h

libserialdata_la_SOURCES=	blockhead.pb.cc serialdatareader.cc	\
				serialdatawriter.cc
libserialdata_la_LDFLAGS=	-version-info ${LIBRARY_VERSION}
libserialdata_la_LIBADD=	${AC_LIBS}

read_write_test_SOURCES=	read_write_test.cc
read_write_test_LDADD=		${GTEST_LIBS} ${lib_LTLIBRARIES}

CLEANFILES=	blockhead.pb.cc blockhead.pb.h

blockhead.pb.cc blockhead.pb.h: blockhead.proto
	protoc --cpp_out=. $<
