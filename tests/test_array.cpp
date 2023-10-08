
#include <QtTest/QtTest>
#include "../src/qjson.h"

using namespace QJSON;

class JsonArrayTest : public QObject
{
	Q_OBJECT

private slots:
	void testAdd()
	{
		Json arr(JsonType::Array);
		QVERIFY(arr.getValueType() == "Array");

		arr.add(1).add(true).add(89.9023).add(9876543210123456)		//max number length
			.add("This is a string value.").add(nullptr).add(Json({{"first","n01"}, {"second", "n02"}}));
		QCOMPARE(arr.toString(), 
			"[1,true,89.9023,9876543210123456,\"This is a string value.\",null,{\"first\":\"n01\",\"second\":\"n02\"}]");

		arr = Json("[    \"JSON Test Pattern pass1\",    {\"object with 1 member\":[\"array with 1 element\"]},    {},    [],    -42]");
		QCOMPARE(arr.toString(), "[\"JSON Test Pattern pass1\",{\"object with 1 member\":[\"array with 1 element\"]},{},[],-42]");
	
		arr = Json("[12345.6789e-7,12345.6789e-3,12345.6789e2,12345.6789e8]");
		QCOMPARE(arr.toString(), "[0.00123456789,12.3456789,1234567.89,1234567890000]");

		Json obj{ {"number", 1}, {"string", "a string"}, {"sub obj","{\"first\":\"one\",\"second\":\"two\"}"} };
		arr.clear().add({ 1,nullptr,true,"another string" }).concat(obj);
		QCOMPARE(arr.toString(), "[1,null,true,\"another string\",1,\"a string\",{\"first\":\"one\",\"second\":\"two\"}]");

		Json arr2(JsonType::Array);
		arr2.add({"one", "two", "three"});
		arr.concat(arr2);
		QCOMPARE(arr.toString(), "[1,null,true,\"another string\",1,\"a string\",{\"first\":\"one\",\"second\":\"two\"},\"one\",\"two\",\"three\"]");
		QVERIFY(arr.size() == 10);
	
		Json rs = arr[0];
		QCOMPARE(rs.getValueType(), "Number");
		QCOMPARE(rs.toDouble(), 1);

		rs = arr[1];
		QCOMPARE(rs.getValueType(), "Null");
		QCOMPARE(rs.isNull(), true);

		rs = arr[2];
		QCOMPARE(rs.getValueType(), "True");
		QCOMPARE(rs.toBool(), true);

		rs = arr[6];
		QCOMPARE(rs.getValueType(), "Object");
		QCOMPARE(rs.toString(), "{\"first\":\"one\",\"second\":\"two\"}");

		rs = arr[6]["second"];
		QCOMPARE(rs.getValueType(), "String");
		QCOMPARE(rs.toString(), "two");

		rs = arr[10];
		QCOMPARE(rs.getValueType(), "Error");

		rs = arr.take(6);
		QCOMPARE(rs["first"].toString(), "one");
		QCOMPARE(arr.toString(), "[1,null,true,\"another string\",1,\"a string\",\"one\",\"two\",\"three\"]");

		rs = arr.slice(2, 6);			//get 2,3,4,5
		QCOMPARE(rs.toString(), "[true,\"another string\",1,\"a string\"]");
		rs = arr.slice(6);				// get 6-> last
		QCOMPARE(rs.toString(), "[\"one\",\"two\",\"three\"]");

		rs = arr.takes(2, 6);			//take 2,3,4,5
		QCOMPARE(rs.toString(), "[true,\"another string\",1,\"a string\"]");
		QCOMPARE(arr.toString(), "[1,null,\"one\",\"two\",\"three\"]");

		QCOMPARE(arr.first().toDouble(), 1);
		QCOMPARE(arr.last().toString(), "three");

		arr.insert(1, "insert a element");
		QCOMPARE(arr.toString(), "[1,\"insert a element\",null,\"one\",\"two\",\"three\"]");

		arr.insert(11, "Out of index range!");		//do nothing, when index out of index range
		QCOMPARE(arr.toString(), "[1,\"insert a element\",null,\"one\",\"two\",\"three\"]");

		arr.clear().push(5);
		QCOMPARE(arr.toString(), "[5]");
		arr.push_back(6);
		QCOMPARE(arr.toString(), "[5,6]");
		arr.push_front(4);
		QCOMPARE(arr.toString(), "[4,5,6]");
		arr.push_front(Json({ {"num1",1},{"num2",2} }));
		QCOMPARE(arr.toString(), "[{\"num1\":1,\"num2\":2},4,5,6]");
		arr.push_back(Json("[7,8,9]"));
		QCOMPARE(arr.toString(), "[{\"num1\":1,\"num2\":2},4,5,6,[7,8,9]]");

		arr.pop();
		QCOMPARE(arr.toString(), "[{\"num1\":1,\"num2\":2},4,5,6]");
		arr.pop_front();
		QCOMPARE(arr.toString(), "[4,5,6]");
		arr.pop_front();
		QCOMPARE(arr.toString(), "[5,6]");
		arr.pop_back();
		QCOMPARE(arr.toString(), "[5]");
		arr.pop_back();
		QCOMPARE(arr.isEmpty(), true);

	}

};

QTEST_MAIN(JsonArrayTest)
#include "test_array.moc"