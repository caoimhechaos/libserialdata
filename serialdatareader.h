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
	// Creates a new serial data reader exception with the reason
	// given in "msg".
	SerialDataReaderException(const QString& msg);
	SerialDataReaderException(const std::string& msg);

	// Exception destructor. Whatever.
	virtual ~SerialDataReaderException() noexcept (true);

	// Return the reason string of the exception.
	QString String() throw ();

protected:
	QString msg_;
};

class SerialDataReaderCorruptionException : public SerialDataReaderException
{
public:
	// Creates the new exception indicating data corruption has
	// been discovered. "msg" is set to a string saying just that.
	SerialDataReaderCorruptionException(const std::string& msg);

	// Exception destructor. Whatever.
	virtual ~SerialDataReaderCorruptionException() noexcept (true);
};

class SerialDataReaderEndReachedException : public SerialDataReaderException
{
public:
	// Creates the new exception indicating that the end of data has
	// been reached.
	SerialDataReaderEndReachedException();

	// Exception destructor. Whatever.
	virtual ~SerialDataReaderEndReachedException() noexcept (true);
};

// Reader for chunks of data which need to be kept in specified
// lengths. Typically, the chunks would be protocol buffers, but it can
// be any arbitrary-length byte field.
class SerialDataReader
{
public:
	// Create a new serial data reader reading from "file". If
	// "file" cannot be opened, a SerialDataReaderException will
	// be thrown.
	SerialDataReader(QString file) throw (SerialDataReaderException);

	// Close the file and end the reader. This will lose all state
	// on where in the file you were reading.
	virtual ~SerialDataReader() throw ();

	// Read the next record in byte form. This does not attempt to
	// decode anything. The amount of bytes returned will be exactly the
	// amount of bytes written originally.
	//
	// If no data is available, returns an empty QByteArray.
	// If the file could not be read, throws a
	// SerialDataReaderException.
	// If the data does not match the CRC32 checksum, a
	// SerialDataReaderCorruptionException is thrown.
	QByteArray ReadRecord(int64_t* offset = 0);

	// Read the record at the position "pos". Otherwise, it behaves
	// just like ReadRecord.
	QByteArray ReadRecordAt(int64_t pos);

protected:
	QString path_;
	QFile handle_;
};

// Proxy class for SerialDataReader to automatically decode protocol
// buffer messages.
template <class T>
class SerialMessageReader
{
public:
	// See the constructor of SerialDataReader.
	SerialMessageReader(QString file) throw (SerialDataReaderException)
	: sr_(file) {}

	// See the destructor of SerialDataReader.
	virtual ~SerialMessageReader() throw () {}

	// Read the next protocol buffer of type "T" which can be
	// found in the input file. If non-NULL, "offset" will be set to the
	// offset of the record in the file.
	T ReadRecord(int64_t* offset = 0)
	{
		QByteArray ba = sr_.ReadRecord(offset);
		T msg;

		if (!msg.ParseFromArray(ba.constData(), ba.length()))
			throw new SerialDataReaderException(
					QString("Data not in requested "
						"format!"));

		return msg;
	}

	// Read the record at the position "pos". Otherwise, it behaves
	// just like ReadRecord.
	T ReadRecordAt(int64_t pos)
	{
		QByteArray ba = sr_.ReadRecordAt(pos);
		T msg;

		if (!msg.ParseFromArray(ba.constData(), ba.length()))
			throw new SerialDataReaderException(
					QString("Data not in requested "
						"format!"));

		return msg;
	}

protected:
	SerialDataReader sr_;
};

}  // namespace serialdata

#endif /* SERIALDATA_SERIALDATAREADER_H */
