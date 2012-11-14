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

#include <string>
#include <exception>
#include <QtCore/QFile>
#include <QtCore/QString>

#include <arpa/inet.h>

#include <google/protobuf/message.h>

#include "serialdatawriter.h"

#include <zlib.h>

namespace serialdata
{

SerialDataWriterException::SerialDataWriterException(const std::string& msg)
: msg_(msg.c_str())
{
}

SerialDataWriterException::~SerialDataWriterException() noexcept (true)
{
}

QString
SerialDataWriterException::String() throw ()
{
	return msg_;
}

SerialDataWriter::SerialDataWriter(QString file)
	throw (SerialDataWriterException)
: path_(file), handle_(file), checksums_(false)
{
	if (!handle_.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		throw new SerialDataWriterException("Unable to open " +
				file.toStdString());
	}
}

SerialDataWriter::~SerialDataWriter() throw ()
{
	handle_.close();
}

void
SerialDataWriter::WriteData(const QByteArray& data)
	throw (SerialDataWriterException)
{
	uint32_t length = htonl(data.length());
	uint32_t checksum = 0UL;
	QByteArray writerdata(reinterpret_cast<char*>(&length), 4);

	if (checksums_)
	{
		crc32(checksum, (unsigned char*) data.constData(), data.length());

		checksum = htonl(checksum);
	}

	writerdata.append(reinterpret_cast<char*>(&checksum), 4);
	writerdata.append(data);

	qint64 written = handle_.write(writerdata);
	if (written < data.length())
		throw new SerialDataWriterException(
				handle_.errorString().toStdString());
}

void
SerialDataWriter::WriteRecord(const google::protobuf::Message* rec)
	throw (SerialDataWriterException)
{
	std::string msg = rec->SerializeAsString();
	QByteArray dest(msg.c_str(), msg.length());

	WriteData(dest);
}

void
SerialDataWriter::SetWriteChecksums(bool do_write_checksums) throw ()
{
	checksums_ = do_write_checksums;
}

}  // namespace serialdata
