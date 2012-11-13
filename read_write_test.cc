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

TEST_F(ReadWriteTest, WriteDataTempFileAndReadBack)
{
	QTemporaryFile tf;
	QByteArray ba("test string one");
	tf.open();
	tf.close();
	SerialDataWriter writer(tf.fileName());

	writer.WriteData(ba);

	ba.clear();
	ba.append("test string two");
	writer.WriteData(ba);

	ba.clear();
	ba.append("test string three");
	writer.WriteData(ba);

	ba.clear();
	try {
		SerialDataReader reader(tf.fileName());
		ba = reader.ReadRecord();
		EXPECT_EQ("test string one", std::string(ba.constData()));

		ba = reader.ReadRecord();
		EXPECT_EQ("test string two", std::string(ba.constData()));

		ba = reader.ReadRecord();
		EXPECT_EQ("test string three", std::string(ba.constData()));
	}
       	catch (SerialDataReaderException* ex)
       	{
		GTEST_FAIL() << ex->String().toStdString();
	}
}

}  // namespace testing
}  // namespace serialdata
