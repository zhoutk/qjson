# QJSON   &emsp;&emsp;  [中文介绍](README_CN.md)  

## Introduce
QJSON is a candidates to [ZJSON](https://gitee.com/zhoutk/zjson.git), I make it to replace ZJSON when it's need stabilization or lower version c++ using Qt5::Core.

## Design ideas  
Using Qt5::Core to realize my goals. Maintain the same interface between the two libraries. QJSON add chain calling on member operation function. ZJSON will add later.

## Project progress
At present, the project has completed most of functions. Please refer to the task list for details. 

task list：
- [x] constructor(Object & Array)
- [x] constructor(values)
- [x] JSON serializable constructor
- [x] copy constructor
- [x] initializer_list constructor
- [x] destructor
- [x] operator=
- [x] operator==
- [x] operator!=
- [x] operator[]
- [x] contains
- [x] getValueType
- [x] getAllKeys
- [x] add（add subitems & add items to array rapidly）
- [x] toString(generate josn string)
- [x] toInt、toDouble、toFalse
- [x] toVector
- [x] isError、isNull、isArray
- [x] parse - from Json string to Json object
- [x] extend - Json
- [x] concat - Json 
- [x] push_front - Json
- [x] push_back - Json
- [x] pop_front - Json
- [x] pop_back - Json
- [x] take - Json
- [x] first -  get the first of array
- [x] last -  get the last of array
- [x] slice - Json
- [x] insert - Json
- [x] clear
- [x] Remove 
- [x] RemoveFirst - remove the first of array
- [x] RemoveLast - remove the last of array
- [x] std::move
  
## Data structure

### Json node type   
> For internal use, the data type is only used inside the Json class
```
enum Type {
    Error,                //error or a invalid Json
    False,                //Json value type - false
    True,                 //Json value type - true
    Null,                 //Json value type - null
    Number,               //Json value type - numerial
    String,               //Json value type - string
    Object,               //Json object type
    Array                 //Json object type
};
```
### Json node define
```
class Json {
    Type type;
    QJsonDocument* _obj_; //data of object or array
    QString vdata;        //data of number or string
}
```
## Interface
Object type, only support Object and Array.
```
enum class JsonType
{
    Object = 6,
    Array = 7
};
```
    
## Examples
```
    Json subObject{{"math", 99},{"str", "a string."}};   
    Json mulitListObj{{"fkey", false},{"strkey","ffffff"},{"num2", 9.98}, {"okey", subObject}};
    Json subArray(JsonType::Array);                 
    subArray.add({12,13,14,15});            

    Json ajson(JsonType::Object);                
    std::string data = "kevin";                     
    ajson.add("fail", false)          // chain call
         .add("name", data);              
         .add("school-en", "the 85th.");   
    ajson.add("age", 10);                  
    ajson.add("scores", 95.98);            
         .add("nullkey", nullptr);         

    Json sub;                                  
    sub.add("math", 99);                 
    ajson.addValueJson("subJson", sub);           

    Json subArray(JsonType::Array);              
    subArray.add("I'm the first one.");   
    subArray.add("two", 2);               
    
    Json sub2;                            
    sub2.add("sb2", 222);

    subArray.addValueJson("subObj", sub2);         
    
    ajson.addValueJson("array", subArray);         

    std::cout << "ajson's string is : " << ajson.toString() << std::endl;   

    string name = ajson["name"].toString();        
    int oper = ajson["sb2"].toInt();               
    Json operArr = ajson["array"];                 
    string first = ajson["array"][0].toString();   
```
result of mulitListObj：
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
result of  ajson：
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
Detailed description, please look over codes of unit test in the tests catalogue.

## Project site
```
https://gitee.com/zhoutk/qjson
or
https://github.com/zhoutk/qjson
```

## run guidance
The project is built in vs2019, gcc7.5, clang12.0 success.  
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

## Associated projects

> [zjson](https://gitee.com/zhoutk/qjson.git) (Maintain the same interface only c++17)
```
https://gitee.com/zhoutk/zjson
or
https://github.com/zhoutk/zjson
```