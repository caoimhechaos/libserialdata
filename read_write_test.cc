#include <gtest/gtest.h>

#include <QtCore/QString>
#include <QtCore/QTemporaryFile>

#include "blockhead.pb.h"
#include "serialdatareader.h"
#include "serialdatawriter.h"

namespace serialdata
{
namespace testing
{
class ReadWriteTest : public ::testing::Test
{
};

TEST_F(ReadWriteTest, SimpleReaderTest)
{
	QByteArray ba;
	SerialDataReader reader("testdata/vicky.ser");
	EXPECT_NO_THROW(ba = reader.ReadRecord());
	EXPECT_EQ("hey vicky!", std::string(ba.constData()));
}

TEST_F(ReadWriteTest, WriteDataTempFileAndReadBack)
{
	QTemporaryFile tf;
	QByteArray ba("test string one");
	tf.open();
	tf.close();
	SerialDataWriter* writer = new SerialDataWriter(tf.fileName());

	writer->WriteData(ba);

	ba.clear();
	ba.append("test string two");
	writer->WriteData(ba);

	ba.clear();
	ba.append("test string three");
	writer->WriteData(ba);
	delete writer;

	ba.clear();
	SerialDataReader reader(tf.fileName());
	EXPECT_NO_THROW(ba = reader.ReadRecord());
	EXPECT_EQ("test string one", std::string(ba.constData()));

	EXPECT_NO_THROW(ba = reader.ReadRecord());
	EXPECT_EQ("test string two", std::string(ba.constData()));

	EXPECT_NO_THROW(ba = reader.ReadRecord());
	EXPECT_EQ("test string three", std::string(ba.constData()));
}

}  // namespace testing
}  // namespace serialdata
