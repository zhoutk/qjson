#pragma once

#include "proc.h"

namespace QJSON {

	class Json final {
		public:
		template <typename T> bool addSubitem(const QString& name, const T& value)
		{
			jPtr = getJsonProcFunc();		//return nullptr but Object and Array
			if (jPtr) {
				QVariant data = value;
				jPtr->appendValue(this->_obj_, name, &data);
				return true;
			}
			else {
				return false;
			}
		};

		Json() : Json(JsonType::Object) {}

		Json(JsonType type) : type((Type)type), jPtr(nullptr) {
			_obj_ = new QJsonDocument;
		}

		explicit Json(std::nullptr_t) {
			new (this)Json();
			this->type = Type::Null;
		}

		explicit Json(bool data) {
			new (this)Json();
			this->type = data ? Type::True : Type::False;
		}

		Json(double data) {
			new (this)Json();
			this->type = Type::Number;
			vdata = QString::number(data);
		}

		Json(std::string data) {
			new (this)Json(QByteArray(data.c_str()));
		}

		Json(QString data) {
			new (this)Json(QByteArray(data.toUtf8()));
		}

		Json(const char* data) {
			new (this)Json(QByteArray(data));
		}

		Json(QByteArray data) {
			QJsonParseError json_error;
			QJsonDocument* jsonDocument = new QJsonDocument(QJsonDocument::fromJson(data, &json_error));
			if (json_error.error == QJsonParseError::NoError) {
				_obj_ = jsonDocument;
				this->type = jsonDocument->isObject() ? Type::Object : Type::Array;
			}
			else {
				_obj_ = new QJsonDocument;
				this->type = Type::String;
				vdata = data;
			}
		}

		bool addValueJson(const QString& name, const Json& obj){
			return true;
		}

		QString toString(){
			switch (this->type)
			{
			case Type::Array:
			case Type::Object:
				return this->_obj_->toJson(QJsonDocument::Compact);
			case Type::Number:
			case Type::String:
				return this->vdata;
			case Type::True:
				return "true";
			case Type::False:
				return "false";
			case Type::Null:
				return "null";
			default:
				return "";
			}
		}

		~Json() {
			if (_obj_)
				delete _obj_;
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
		QString vdata;

		JsonProc* jPtr;
		
		JsonProc* getJsonProcFunc() {
			if (this->type == Type::Object) {
				return ObjectProc::Instance();
			}
			else if (this->type == Type::Array) {
				return ArrayProc::Instance();
			}
			else {
				return nullptr;
			}
		}

		Json(Type type) {
			new (this)Json();
			this->type = type;
		}

	};

	template <> bool Json::addSubitem(const QString& name, const std::nullptr_t&) {
		jPtr = getJsonProcFunc();
		if (jPtr) {
			jPtr->appendValue(this->_obj_, name, nullptr);
			return true;
		}
		else {
			return false;
		}
	}
	template <> bool Json::addSubitem(const QString& name, const Json& value) {
		return addValueJson(name, value);
	}

}