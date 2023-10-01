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
		Json() : type(Type::Object) {
			_obj_ = new QJsonDocument;
		}

		Json(JsonType type) : type((Type)type){
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
			JsonProc* jp = getJsonProcFunc();
			if(jp){
				QVariant data = value;
				jp->appendValue(this->_obj_, name, data);
				delete jp;
				return true;
			}else{
				return false;
			}
		}

		QString toString(){
			return this->_obj_->toJson(QJsonDocument::Compact);;
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

		class JsonProc {
			public:
			virtual void appendValue(QJsonDocument*& obj, const QString& name, const QVariant& value) = 0;
		};

		class ObjectProc : public JsonProc {
			public:
			void appendValue(QJsonDocument*& obj, const QString& name, const QVariant& value){
				QJsonObject json = obj->object();
				if (json.contains(name))
					json.remove(name);
				json.insert(name, value.toString());
				delete obj;
				obj = new QJsonDocument(json);
			}
		};

		class ArrayProc : public JsonProc {
			public:
			void appendValue(QJsonDocument*& obj, const QString& name, const QVariant& value){
				QJsonArray json = obj->array();
				json.push_back(value.toString());
				delete obj;
				obj = new QJsonDocument(json);
			}
		};

		JsonProc* getJsonProcFunc() {
			if (this->type == Type::Object) {
				return new ObjectProc;
			}
			else if (this->type == Type::Array) {
				return new ArrayProc;
			}
			else {
				return nullptr;
			}
		}
	};

}