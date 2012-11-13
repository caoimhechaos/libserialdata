/*-
 * Copyright (c) 2012 Caoimhe Chaos <caoimhechaos@protonmail.com>,
 *                    Ancient Solutions. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions  of source code must retain  the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions  in   binary  form  must   reproduce  the  above
 *    copyright  notice, this  list  of conditions  and the  following
 *    disclaimer in the  documentation and/or other materials provided
 *    with the distribution.
 *
 * THIS  SOFTWARE IS  PROVIDED BY  ANCIENT SOLUTIONS  AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO,  THE IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS
 * FOR A  PARTICULAR PURPOSE  ARE DISCLAIMED.  IN  NO EVENT  SHALL THE
 * FOUNDATION  OR CONTRIBUTORS  BE  LIABLE FOR  ANY DIRECT,  INDIRECT,
 * INCIDENTAL,   SPECIAL,    EXEMPLARY,   OR   CONSEQUENTIAL   DAMAGES
 * (INCLUDING, BUT NOT LIMITED  TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE,  DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT  LIABILITY,  OR  TORT  (INCLUDING NEGLIGENCE  OR  OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SERIALDATA_SERIALDATAREADER_H
#define SERIALDATA_SERIALDATAREADER_H 1

namespace serialdata
{

class SerialDataReaderException : public std::exception
{
public:
	SerialDataReaderException(const std::string& msg);
	virtual ~SerialDataReaderException() noexcept (true);

	QString String();

protected:
	QString msg_;
};

class SerialDataReader
{
public:
	SerialDataReader(QString file) throw (SerialDataReaderException);
	virtual ~SerialDataReader() throw ();

	QByteArray ReadRecord() throw (SerialDataReaderException);

protected:
	QString path_;
	QFile handle_;
};

template <class T>
class SerialMessageReader
{
public:
	SerialMessageReader(QString file) throw (SerialDataReaderException);
	virtual ~SerialMessageReader() throw ();

	T& ReadRecord() throw (SerialDataReaderException);

protected:
	SerialDataReader sr_;
};

}  // namespace serialdata

#endif /* SERIALDATA_SERIALDATAREADER_H */
