//#pragma execution_character_set("utf-8")

#include "qjson.h"
#include <QDebug>

using namespace QJSON;

int main(int argc, char* argv[])
{
	//Json rs(JsonType::Object);
	//Json a = Json("[\"a\",\"abc\"]");
	//Json one(JsonType::Array);
	//one.add("32624u7248367");

	////rs.add("first of one", one);

	//one.add(nullptr).add(0).add(123);
	//one.insert(0, 11.1).add("double", 211.654321).add("boolF", false);
	//one.add("boolT", true).insert(1, "number one");

	//rs.add({"a object", false, nullptr, 11.90000018, true}).add(a).push_back(a).insert(4, Json("next value"));
	Json subObject { {"math", 99}, { "str", "a string." } };
	Json mulitListObj{ 
		{"fkey", false},
		{"nullkey", nullptr},
		{"strkey","ffffff"},
		{"num1", 5},
		{"num2", 9.98},
		{"array", "not support"},
		{"objStr", "{\"first\":\"one\",\"second\":\"two\"}"},
		{"okey", subObject} };
	std::cout << mulitListObj.toString();
	//QString key = "num1";
	//qDebug() << "  ---  " << one.toString();

	getchar();
	// Json sub(JsonType::Array);
	// sub.concat("1");
	// sub.concat("2");
	// one.add("first", sub);
	// rs.add(one);

	// one.clear();
	// sub.clear();
	// sub.concat("3");
	// sub.concat("4");
	// one.add("second", sub);
	// rs.add(one);
	// std::string szData = "{\"data\":{\"pid\":19692},\"type\":11024}";
	// Json UserData(szData);
	// std::cout << "type:" << UserData["type"].toInt() << UserData.toString() << std::endl;

	// string strBase = "{\"array\":[\"first\",null],\"true\":true,\"subobj\":{\"field01\":\"obj01\",\"subNumber\":99,\"null\":null},\"age\":10}"; 
	// Json rs(strBase);

	// std::cout << "parse a object string : " << rs.toString() << std::endl;
	//string str = "{\"array\":[\"first\",90.12387,null,2,true],\"true\":true,\"false\":false,\"null\":null,\"age\":18,\"score\":12.3456,\"name\":\"kevin\"}";
	//string str = "{\"age\":10,\"array\":[\"first\",null],\"true\":true}"; //,\"true\":true
	//string str = "{\"long\":123,\"longlong\":56789,\"sex\":true,\"name\":\"kevin\",\"school-cn\":\"第八十五中学\",\"subObjct\":{\"first\":\"this is the first.\",\"second obj\":{\"sub2-1\":\"the second sub object.\",\"sub2-2\":\"the second field.\"},\"a number\":666},\"school-en\":\"the 85th.\",\"age\":10,\"scores\":95.98,\"classroom\":9.012345,\"index\":54,\"nullkey\":null}";
	//string str = "{\"subObjct\":{\"first\":\"this is the first.\",\"second obj\":{\"array01\":[1,2,3,{\"sub3\":\"sub3-1\",\"sub3-next\":true}]}}}";
	//string str = "[    \"JSON Test Pattern pass1\",    {\"object with 1 member\":[\"array with 1 element\"]},    {},    [],    -42]";
	//string str = "[    \"JSON Test Pattern pass1\",    {\"object with 1 member\":[\"array with 1 element\"]},    {},    [],    -42,    true,    false,    null,    {        \"integer\": 1234567890,        \"real\": -9876.543210,        \"e\": 0.123456789e-12,        \"E\": 1.234567890E+34,        \"\":  23456789012E66,        \"zero\": 0,        \"one\": 1}]";
	// Json subArray(JsonType::Array);
	// subArray.add(2);
	// subArray.add({12,13,14,15});
	// std::vector<Json> sub = subArray.toVector();
	// std::vector<Json> arr;
	// Json sub;
	// sub.add("yuwen", 66);
	// sub.add("draw", 11);
	// sub.add("hx", 90);
	// //Json subMove = std::move(sub);
	// arr.push_back(sub);
	// Json sub2(JsonType::Array);
	// sub2.add("math", 99);
	// sub2.add("str", 100);
	// arr.push_back(sub2);
	// Json sub3;
	// sub3.add("music", 95);
	// sub3.add("draw", 88);
	// arr.push_back(sub3);

	// Json rs;
	// rs.add("data", arr);
	//  subArray.concat(sub2);
	 //Json sub{{"fkey", false},{"strkey","ffffff"},{"nkey", nullptr}, {"n1", 2}, {"num2", 9.98}, {"okey", subArray}};
	// sub.add("this is the first.");
	// Json subb(JsonType::Array);
	// subb.add("the second sub object.");
	// subb.add("the second field.");
	// sub.add("second obj", subb);
	// std::cout << "parse a object string : " << rs.toString() << std::endl;
	// str = "[\"true\":true,\"false\":false,\"null\":null,\"age\":18,\"score\":12.3456,\"name\":\"kevin\"]";
	// Json arrStr(str);
	// std::cout << "parse a array string : " << arrStr.toString() << std::endl;
	// for(int i = 0; i < 1; i++) {
	// 	Json ajson(JsonType::Object);
	// 	std::string data = "kevin";
	// 	float f = 9.01234567;
	// 	long l = 123;
	// 	long long ll = 56789;
	// 	ajson.add("long", l);
	// 	ajson.add("longlong", ll);
	// 	ajson.add("sex", true);
	// 	ajson.add("falt", false);
	// 	ajson.add("name", data);
	// 	ajson.add("school-cn", "第八十五中学");
	// 	ajson.add("school-en", "the 85th.");
	// 	ajson.add("age", 10);
	// 	ajson.add("scores", 95.98);
	// 	ajson.add("classroom", f);
	// 	ajson.add("index", '6');
	// 	ajson.add("nullkey", nullptr);

	// 	Json sub;
	// 	sub.add("math", 99);
	// 	ajson.addValueJson("subJson", sub);

	// 	Json subArray(JsonType::Array);
	// 	subArray.add("first", "I'm the first one.");
	// 	subArray.add("two", 2);
	// 	Json subb;
	// 	subb.add("sbbbbb", "bbbbbbb");
	// 	Json littleArray(JsonType::Array);
	// 	littleArray.add(888);
	// 	littleArray.add(999);
	// 	Json sub2;
	// 	sub2.add("sb2", "second");
	// 	littleArray.addValueJson(sub2);
	// 	subb.addValueJson("arr", littleArray);
	// 	subArray.addValueJson("subObj", subb);
	// 	ajson.addValueJson("array", subArray);

	// 	ajson.add("scores", 95.98);
	// 	std::cout << "ajson's string is : " << ajson.toString() << std::endl;

	// 	Json oper = ajson["sb2"];
	// 	Json operArr = ajson["arr"];
	// 	Json operArr2 = ajson["arr"][2];
	// 	Json operBool1 = ajson["sex"];
	// 	Json operBool2 = ajson["fail"];
	// 	Json nullValue = ajson["nullkey"];

	// 	std::cout << "[sb2] operator : " << oper.toString() << std::endl;
	// 	std::cout << "[arr] operator : " << operArr.toString() << std::endl;
	// 	std::cout << "[arr2] operator : " << operArr2.toString() << std::endl;
	// 	std::cout << "[int] operator : " << oper.toInt() << std::endl;
	// 	std::cout << "[float] operator : " << oper.toFloat() << std::endl;
	// 	std::cout << "[double] operator : " << oper.toDouble() << std::endl;
	// 	std::cout << "[true] operator : " << operBool1.toBool() << std::endl;
	// 	std::cout << "[false] operator : " << operBool2.toBool() << std::endl;
	// 	std::cout << "[null] operator : " << nullValue.isNull() << std::endl;

	// }

	// int a = 1;

	// Json aClone(subb);

	// Json aCp = subArray;

	// std::cout << std::endl;
	// std::cout << "ajson's string is : " << ajson.toString() << std::endl;
	// std::cout << "aClone's string is : " << aClone.toString() << std::endl;
	// std::cout << "aCp's string is : " << aCp.toString() << std::endl;
}