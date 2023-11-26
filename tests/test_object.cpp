
#include <QtTest/QtTest>
#include "../src/qjson.h"

using namespace QJSON;

class JsonObjectTest : public QObject
{
	Q_OBJECT

private slots:
	void testObjAdd()
	{
		Json obj;
		QVERIFY(obj.getValueType() == "Object");
		obj.add("FalseObj", false);

		QCOMPARE(obj.toString(), "{\"FalseObj\":false}");
		Json ele = obj.take("FalseObj");

		QCOMPARE(ele.toString(), "false");
		obj.add("TrueObj", true);
		QCOMPARE(obj.toString(), "{\"TrueObj\":true}");
		obj.clear()
			.add("NullObj", nullptr)
			.add("IntObj", 1)
			.add("CharObj", 'A')
			.add("DoubleObj1", 123.56)
			.add("DoubleObj2", 758.000001001)
			.add("DoubleObj3", 65536.0123456789)		//max numbers of decimal
			.add("DoubleObj4", 987654321.1234567);
		QCOMPARE(obj.toString(),
			"{\"CharObj\":65,\"DoubleObj1\":123.56,\"DoubleObj2\":758.000001001,\"DoubleObj3\":65536.0123456789,\"DoubleObj4\":987654321.1234567,\"IntObj\":1,\"NullObj\":null}");

		Json sub{ {"number", 1}, {"string", "a string"} };
		obj = Json({									//object initializer_list ctor
			{"first","this is the first."},
			{"sub obj","{\"first\":\"one\",\"second\":\"two\"}"},
			{"arr1", "[1,2,3]"},
			{"sub obj2", sub},
			});
		Json arr(JsonType::Array);
		arr.add({ 4,5,6 });								//array initializer_list add
		obj.add("array2", arr);
		QCOMPARE(obj.toString(),
			"{\"arr1\":[1,2,3],\"array2\":[4,5,6],\"first\":\"this is the first.\",\"sub obj\":{\"first\":\"one\",\"second\":\"two\"},\"sub obj2\":{\"number\":1,\"string\":\"a string\"}}");
	}

	void testObjFind()
	{
		Json obj = Json("{\"arr1\":[1,2,3],\"sub obj2\":{\"number\":1,\"string\":\"a string\"},\"array2\":[4,5,6],\"first\":\"this is the first.\",\"sub obj\":{\"first\":\"one\",\"second\":\"two\"}}");
		QCOMPARE(obj.toString(), "{\"arr1\":[1,2,3],\"array2\":[4,5,6],\"first\":\"this is the first.\",\"sub obj\":{\"first\":\"one\",\"second\":\"two\"},\"sub obj2\":{\"number\":1,\"string\":\"a string\"}}");
	
		Json ele = obj["first"];
		QCOMPARE(ele.getValueType(), "String");
		QCOMPARE(ele.toString(), "this is the first.");

		ele = obj["sub obj"];
		QCOMPARE(ele.getValueType(), "Object");
		QCOMPARE(ele.toString(), "{\"first\":\"one\",\"second\":\"two\"}");

		ele = obj["sub obj"]["second"];
		QCOMPARE(ele.getValueType(), "String");
		QCOMPARE(ele.toString(), "two");

		ele = obj["arr1"];
		QCOMPARE(ele.getValueType(), "Array");
		QCOMPARE(ele.toString(), "[1,2,3]");

		ele = obj["array2"][1];
		QCOMPARE(ele.getValueType(), "Number");
		QCOMPARE(ele.toString(), "5");

		std::vector<std::string> ls = obj.getAllKeys();
		QStringList keyList;
		for (auto al : ls)
			keyList << QString::fromStdString(al);
		QCOMPARE(keyList.join(','), "arr1,array2,first,sub obj,sub obj2");

		obj.remove("sub obj");
		ls = obj.getAllKeys();
		keyList.clear();
		for (auto al : ls)
			keyList << QString::fromStdString(al);
		QCOMPARE(keyList.join(','), "arr1,array2,first,sub obj2");

		obj.remove("arr1");
		QCOMPARE(obj.toString(), "{\"array2\":[4,5,6],\"first\":\"this is the first.\",\"sub obj2\":{\"number\":1,\"string\":\"a string\"}}");

		Json sub { {"number", 1}, {"string", "a string"}, {"first", "replace first content"} };
		obj.extend(sub);
		QCOMPARE(obj.toString(), "{\"array2\":[4,5,6],\"first\":\"replace first content\",\"number\":1,\"string\":\"a string\",\"sub obj2\":{\"number\":1,\"string\":\"a string\"}}");
	}

};

QTEST_MAIN(JsonObjectTest)
#include "test_object.moc"