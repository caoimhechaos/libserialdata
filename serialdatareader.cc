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

#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QString>

#include <exception>
#include <zlib.h>

#include "serialdatareader.h"
#include "blockhead.pb.h"

namespace serialdata
{

SerialDataReader::SerialDataReader(QString file)
	throw (SerialDataReaderException)
: path_(file), handle_(file)
{
	if (!handle_.open(QIODevice::ReadOnly))
		throw new SerialDataReaderException("Unable to open " +
				file.toStdString() + ": " +
				handle_.errorString().toStdString());
}

SerialDataReader::~SerialDataReader() noexcept (true)
{
	handle_.close();
}

QByteArray
SerialDataReader::ReadRecord() throw (SerialDataReaderException)
{
	SerialDataBlockHead bh;
	QByteArray ra_data = handle_.peek(512);

	if (!bh.ParseFromArray(ra_data.constData(), ra_data.length()))
		throw new SerialDataReaderException("Data not in blockhead "
				"format or too long!");

	if (!handle_.seek(handle_.pos() + bh.ByteSize()))
		throw new SerialDataReaderException("Unable to seek to data: "
				+ handle_.errorString().toStdString());

	ra_data = handle_.read(bh.block_length());
	if (ra_data.length() != bh.block_length())
		throw new SerialDataReaderException("Unable to read data: " +
				handle_.errorString().toStdString());

	if (bh.has_checksum())
	{
		uint32_t state = 0UL;

		crc32(state, (unsigned char*) ra_data.constData(),
			       	ra_data.length());

		if (state != bh.checksum())
			throw new SerialDataReaderException(
					"Data corrupted (CRC32 mismatch)");
	}

	return ra_data;
}

template<class T>
SerialMessageReader<T>::SerialMessageReader(QString file)
	throw (SerialDataReaderException)
: sr_(file)
{
}

template<class T>
SerialMessageReader<T>::~SerialMessageReader() throw ()
{
}

template<class T>
T&
SerialMessageReader<T>::ReadRecord() throw (SerialDataReaderException)
{
	QByteArray ba = sr_.ReadRecord();
	T msg;

	if (!msg.ParseFromArray(ba.constData(), ba.length()))
		throw new SerialDataReaderException("Data not in requested "
				"format!");

	return msg;
}

}  // namespace serialdata
