
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

		obj.add(1).add(true).add(89.9023).add(9876543210123456)		//max number length
			.add("This is a string value.").add(nullptr).add(Json({{"first","n01"}, {"second", "n02"}}));
		QCOMPARE(obj.toString(), 
			"[1,true,89.9023,9876543210123456,\"This is a string value.\",null,{\"first\":\"n01\",\"second\":\"n02\"}]");
	}

};

QTEST_MAIN(JsonArrayTest)
#include "test_array.moc"