#pragma once
//#pragma execution_character_set("utf-8")

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

		bool addValueJson(const QString& name, const Json& obj){
			return true;
		}

		QString toString(){
			return this->_obj_->toJson(QJsonDocument::Compact);
		}

		~Json() {
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

	};

	template <> bool Json::addSubitem(const QString& name, const std::nullptr_t& value) {
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