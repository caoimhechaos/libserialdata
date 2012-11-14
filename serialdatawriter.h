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

#ifndef SERIALDATA_SERIALDATAWRITER_H
#define SERIALDATA_SERIALDATAWRITER_H 1

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace serialdata
{

class SerialDataWriterException : public std::exception
{
public:
	// Create a new serial data writer exception with the reason
	// given in "msg".
	SerialDataWriterException(const std::string& msg);

	// Exception destructor. Whatever.
	virtual ~SerialDataWriterException() noexcept (true);

	// Return the reason string of the exception.
	QString String() throw ();

private:
	QString msg_;
};

class SerialDataWriter
{
public:
	// Create a new serial data writer writing into "file". If the
	// file cannot be opened, throws a SerialDataWriterException.
	// If the file exists, new content will be appended to it.
	SerialDataWriter(QString file) throw (SerialDataWriterException);

	// Close the file and stop writing. Writing can be resumed at any
	// time by opening the file again.
	virtual ~SerialDataWriter() throw ();

	// Write the protocol buffer message "rec" into the file, including
	// the identifying header so it can be read out again.
	void WriteRecord(const google::protobuf::Message* rec)
		throw (SerialDataWriterException);

	// Writes the "data" into the file, prepending it with a nice
	// header for easier handling. The header is completely
	// invisible to the user of this library.
	//
	// The reader will return exactly the amount of bytes put into
	// this method. No concatenation will be done.
	void WriteData(const QByteArray& data)
		throw (SerialDataWriterException);

	// Enable or disable CRC32 checksums for writing the file. If
	// checksums are enabled ("do_write_checksums" is set to
	// true), the next records written will contain a checksum,
	// otherwise they won't.
	//
	// Modifying this settings has no consequence whatsoever for
	// the data which was already written to the file.
	void SetWriteChecksums(bool do_write_checksums) throw ();

private:
	QString path_;
	QFile handle_;
	bool checksums_;
};

} // namespace serialdata

#endif /* SERIALDATA_SERIALDATAWRITER_H */
