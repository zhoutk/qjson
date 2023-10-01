#pragma once

#include <QJsonDocument >
#include <QString>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

namespace QJSON {

	#define GetTypeName(T) check_type<T>()
	#define GetVarTypeName(var) check_type<decltype(var)>()

	template <typename T>
	struct check
	{
		check(QString& out)
		{
	#   if defined(__GNUC__)
			char* real_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
			out = real_name;
			free(real_name);
	#   else
			out = typeid(T).name();
	#   endif
		}
	};

	template <typename T>
	inline QString check_type(void)
	{
		QString str;
		check<T> { str };
		return str;
	}

	enum class JsonType
	{
		Object = 6,
		Array = 7
	};

	class Json final {
		public:
		Json() : type(Type::Object), jPtr(nullptr) {
			_obj_ = new QJsonDocument;
		}

		Json(JsonType type) : type((Type)type), jPtr(nullptr){
			_obj_ = new QJsonDocument;
		}

		Json(QByteArray data) {
			QJsonParseError json_error;
			QJsonDocument jsonDocument = QJsonDocument::fromJson(data, &json_error);
			if (json_error.error == QJsonParseError::NoError) {
				_obj_ = &jsonDocument;
			}
			else {
				type = Type::Error;
			}
		}

		Json(QString jstr) {
			new (this)Json(jstr.toUtf8());
		}

		template <typename T>
		bool addSubitem(const QString &name, const T &value)
		{
			if (this->type == Type::Object || this->type == Type::Array)
			{
				QString typeStr = GetTypeName(T);
				if (typeStr.contains("QJSON::Json"))
				{
					Json temp;
					return addValueJson(name, temp);
				}
				else
				{
					return appendValue(typeStr, name, value);
				}
			}
			else
				return false;
		};

		bool addValueJson(const QString& name, const Json& obj){
			return true;
		}

		template<typename T> bool appendValue(const QString& typeStr, const QString& name, const T& value){
			if(typeStr.contains("QJSON::Json"))
				return false;
			jPtr = getJsonProcFunc();		//return nullptr but Object and Array
			if(jPtr){
				QVariant data = value;
				jPtr->appendValue(this->_obj_, name, data);
				return true;
			}else{
				return false;
			}
		}

		QString toString(){
			return this->_obj_->toJson(QJsonDocument::Compact);;
		}

		~Json() {
			if (_obj_)
				delete _obj_;
			if (oPtr)
				delete oPtr;
			if (aPtr)
				delete aPtr;
		}

		private:
		enum Type {
			Error,
			False,
			True,
			Null,
			Number,
			String,
			Object,
			Array
		};
		Type type;
		QJsonDocument* _obj_;

	public:
		class JsonProc abstract {
			public:
			virtual void appendValue(QJsonDocument*& obj, const QString& name, const QVariant& value) = 0;
		};

		class ObjectProc : public JsonProc {
			public:
			static ObjectProc* Instance(){
				if(appPtr == nullptr)
					appPtr = new ObjectProc;
				return appPtr;
				
			}

			void appendValue(QJsonDocument*& obj, const QString& name, const QVariant& value){
				QJsonObject json = obj->object();
				if (json.contains(name))
					json.remove(name);
				json.insert(name, value.toString());
				delete obj;
				obj = new QJsonDocument(json);
			}

			~ObjectProc(){
				if(appPtr)
					delete appPtr;
			}
			private:
			ObjectProc() {}
			ObjectProc(const ObjectProc &) = delete;
			ObjectProc &operator=(const ObjectProc &) = delete;

			static ObjectProc* appPtr;
		};

		class ArrayProc : public JsonProc {
			public:
			static ArrayProc* Instance(){
				if(appPtr == nullptr)
					appPtr = new ArrayProc;
				return appPtr;
				
			}

			void appendValue(QJsonDocument*& obj, const QString& name, const QVariant& value){
				QJsonArray json = obj->array();
				json.push_back(value.toString());
				delete obj;
				obj = new QJsonDocument(json);
			}

			~ArrayProc(){
				if(appPtr)
					delete appPtr;
			}
			private:
			ArrayProc() {}
			ArrayProc(const ArrayProc &) = delete;
			ArrayProc &operator=(const ArrayProc &) = delete;

			static ArrayProc* appPtr;
		};


		JsonProc* getJsonProcFunc() {
			if (this->type == Type::Object) {
				return oPtr;
			}
			else if (this->type == Type::Array) {
				return aPtr;
			}
			else {
				return nullptr;
			}
		}
		JsonProc* jPtr;
		static ObjectProc* oPtr;
		static ArrayProc* aPtr;
	};

}