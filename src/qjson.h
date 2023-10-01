#pragma once

#include "proc.h"

namespace QJSON {

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


		JsonProc* jPtr;
		static ObjectProc* oPtr;
		static ArrayProc* aPtr;
		
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

	};

}