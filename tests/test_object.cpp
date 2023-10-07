
#include <QtTest/QtTest>
#include "../src/qjson.h"

using namespace QJSON;

class JsonObjectTest : public QObject
{
	Q_OBJECT

private slots:
	void testAdd()
	{
		Json obj;
		obj.add("1", 1);
		QCOMPARE(obj.toString(), "{\"1\":1}");
	}

};

QTEST_MAIN(JsonObjectTest)
#include "test_object.moc"