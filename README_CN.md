# QJSON   &emsp;&emsp;  [English](README.md)  

## 介绍
QJSON 是 [ZJSON](https://gitee.com/zhoutk/zjson.git)的替代库，我花时间写这个库的原因有两个，一是需要c++低版本的场景；二是稳定性（ZJSON技术选择过于激进）优先保证的情况。封装QT5:Core中的相关Json库，与ZJSON保持相同的外部接口，让两个库随时可以按需替换。

## 项目名称说明
Qt项目是本项目稳定的保证，本项目的实质是封装Qt的json库，使调用接口与ZJSON项目保持一致。QJSON在成员操作的相关函数上实现了链式调用，ZJSON后面也会实现。ZJSON准备以状态机来重写json对象识别，并按本项目的模板特化的思路来重构优化。

## 设计思路 
简单的接口函数、简单的使用方法、灵活的数据结构、尽量支持链式操作。使用模板技术，得以完成最简设计，为Json对象子对象的方法只需一个 ———— add，该方法自动识别是值对象还是子Json对象。

## 项目进度
项目目前完成大部分主要功能，具体情况请看任务列表，同时支持windws、linux和mac主流操作系统。  
任务列表：
- [x] 构造函数(Object & Array)
- [x] 构造函数(值)
- [x] JSON字符串反序列化构造函数
- [x] 复制构造函数
- [x] initializer_list构造函数
- [x] 析构函数
- [x] operator=
- [x] operator==
- [x] operator！=
- [x] operator[]
- [x] contains
- [x] getValueType
- [x] getAllKeys
- [x] add（为Json对象增加子对象，为数组快速增加元素）
- [x] toString(生成json字符串)
- [x] toInt、toDouble、toFalse 等值类型转换
- [x] toVector 数组类型转换
- [x] isError、isNull、isArray 等节点类型判断
- [x] parse, 从json字符串生成Json对象
- [x] extend Json - 扩展对象
- [x] concat Json - 数组扩展 
- [x] push_front - 数组压入队首
- [x] push_back - 数组压入队尾
- [x] pop_front - 数组弹出队首
- [x] pop_back - 数组弹出队尾
- [x] take -  获取并删除
- [x] first -  获取数组第一个
- [x] last -  获取数组最后一个
- [x] slice - 数组取出子数组
- [x] insert - 数组插入
- [x] clear - 清空
- [x] Remove - 删除
- [x] RemoveFirst - 删除数组第一个
- [x] RemoveLast - 删除数组最后一个
- [x] std::move语义
  
## 数据结构

### Json 节点类型定义   
（内部使用，数据类型只在Json类内部使用）
```
enum Type {
    Error,                //错误，查找无果，这是一个无效Json对象
    False,                //Json值类型 - false
    True,                 //Json值类型 - true
    Null,                 //Json值类型 - null
    Number,               //Json值类型 - 数字，库中以double类型存储
    String,               //Json值类型 - 字符串
    Object,               //Json类对象类型 - 这是Object嵌套，对象型中只有child需要关注
    Array                 //Json类对象类型 - 这是Array嵌套，对象型中只有child需要关注
};
```
### Json 节点定义
```
class Json {
    Type type;
    QJsonDocument* _obj_; //data of object or array
    QString vdata;        //data of number or string
}
```
## 接口说明
公开的对象类型，json只支持Object与Array两种对象，与内部类型对应（公开类型）。
```
enum class JsonType
{
    Object = 6,
    Array = 7
};
```
    
## 编程示例
简单使用示例
```
    Json subObject{{"math", 99},{"str", "a string."}};   //initializer_list方式构造Json对象

    //initializer_list方式构造Json对象, 并且可以嵌套
	Json mulitListObj{{"fkey", false},{"strkey","ffffff"},{"num2", 9.98}, {"okey", subObject}};

    Json subArray(JsonType::Array);                 //数组对象以initializer_list方式增加元素
	subArray.add({12,13,14,15});             //快速生成 [12,13,14,15] array json

    Json ajson(JsonType::Object);                   //新建Object对象，输入参数可以省略
    std::string data = "kevin";                     
    ajson.add("fail", false)              //增加false值对象
         .add("name", data)               //增加字符串值对象
         .add("school-en", "the 85th.");  //链式调用 
    ajson.add("age", 10);                  //增加number值对象，此处为整数
    ajson.add("scores", 95.98);            //增加number值对象，此处为浮点数，还支持long,long long
         .add("nullkey", nullptr);         //增加null值对象，需要送入nullptr， NULL会被认为是整数0

    Json sub;                                       //新建Object对象
    sub.add("math", 99);                 
    ajson.addValueJson("subJson", sub);             //为ajson增加子Json类型对象，完成嵌套需要

    Json subArray(JsonType::Array);                 //新建Array对象，输入参数不可省略
    subArray.add("I'm the first one.");    //增加Array对象的字符串值子对象
    subArray.add("two", 2);                //增加Array对象的number值子对象，第一个参数会被忽略
    
    Json sub2;                            
    sub2.add("sb2", 222);

    subArray.addValueJson("subObj", sub2);          //为Array对象增加Object类子对象，完成嵌套需求
    
    ajson.addValueJson("array", subArray);          //为ajson增加Array对象，且这个Array对象本身就是一个嵌套结构

    std::cout << "ajson's string is : " << ajson.toString() << std::endl;    //输出ajson对象序列化后的字符串， 结果见下方

    string name = ajson["name"].toString();         //提取key为name的字符串值，结果为：kevin
    int oper = ajson["sb2"].toInt();                //提取嵌套深层结构中的key为sb2的整数值，结果为：222
    Json operArr = ajson["array"];                  //提取key为array的数组对象
    string first = ajson["array"][0].toString();    //提取key为array的数组对象的序号为0的值，结果为：I'm the first one.
```
mulitListObj序列化后结果为：
```
{
    "fkey": false,
    "strkey": "ffffff",
    "num2": 9.98,
    "okey": {
        "math": 99,
        "str": "a string."
    }
}
```
ajson序列化后结果为：
```
{
    "fail": false,
    "name": "kevin",
    "school-en": "the 85th.",
    "age": 10,
    "scores": 95.98,
    "nullkey": null,
    "subJson": {
        "math": 99
    },
    "array": [
        "I'm the first one.",
        2,
        {
            "sb2": 222
        }
    ]
}
```
详情请参看相应的测试用例代码。

## 项目地址
```
https://gitee.com/zhoutk/qjson
或
https://github.com/zhoutk/qjson
```

## 运行方法
该项目在vs2019, gcc7.5, clang12.0下均编译运行正常。
```
git clone https://github.com/zhoutk/qjson
cd qjson
cmake -Bbuild .

---windows
cd build && cmake --build .

---linux & mac
cd build && make

run qjson or ctest
```

## 相关项目

> [zjson](https://gitee.com/zhoutk/zjson.git) (与本项目保持相同的接口，可互换替换)
```
https://gitee.com/zhoutk/zjson
或
https://github.com/zhoutk/zjson
```