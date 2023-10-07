
#include <QtTest/QtTest>
#include "../src/qjson.h"

using namespace QJSON;

class JsonArrayTest : public QObject
{
	Q_OBJECT

private slots:
	void testAdd()
	{
		Json obj(JsonType::Array);
		QVERIFY(obj.getValueType() == "Array");
	}

};

QTEST_MAIN(JsonArrayTest)
#include "test_array.moc"