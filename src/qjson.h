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

		template<typename T> bool addSubitem(const T& value) {
			if (this->type == Type::Array)
				return addSubitem("", value);
			else
				return false;
		}

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
			vdata = QString::number(data, 'f', getDecimalCount(data));
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

		Json(const Json& origin) {
			this->type = origin.type;
			this->vdata = origin.vdata;
			this->_obj_ = new QJsonDocument(*origin._obj_);
		}

		Json(Json&& rhs) {
			this->type = rhs.type;
			this->_obj_ = rhs._obj_;
			this->vdata = rhs.vdata;
			rhs._obj_ = nullptr;
		}

		Json& operator = (const Json& origin) {
			return *(new (this)Json(origin));
		}

		Json& operator = (Json&& rhs) {
			return *(new (this)Json(std::move(rhs)));
		}

		Json operator[](const int& index) {
			Json rs(Type::Error);
			if (this->type == Type::Array) {
				QJsonArray arr = this->_obj_->array();
				if (index >= 0 && index < arr.size()) {
					QJsonValue v = arr[index];
					parseValueToJson(rs, v);
				}
			}
			return rs;
		}

		Json operator[](const QString& key) {
			Json rs(Type::Error);
			if (this->type == Type::Object) {
				QJsonObject obj = this->_obj_->object();
				QJsonValue v = obj[key];
				parseValueToJson(rs, v);
			}
			return rs;
		}

		QString toString() const {
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
		/*	case Type::Null:
				return "null";*/
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

		void parseValueToJson(Json& rs, const QJsonValue& v) {
			switch (v.type())
			{
			case QJsonValue::Array:
				rs.type = Type::Array;
				rs._obj_ = new QJsonDocument(v.toArray());
				break;
			case QJsonValue::Object:
				rs.type = Type::Object;
				rs._obj_ = new QJsonDocument(v.toObject());
				break;
			case QJsonValue::Double:
				rs.type = Type::Number;
				rs._obj_ = new QJsonDocument();
				rs.vdata = QString::number(v.toDouble(), 'f', getDecimalCount(v.toDouble()));
				break;
			case QJsonValue::String:
				rs.type = Type::String;
				rs._obj_ = new QJsonDocument();
				rs.vdata = v.toString();
				break;
			case QJsonValue::Bool:
				rs.type = v.toBool() ? Type::True : Type::False;
				rs._obj_ = new QJsonDocument();
				break;
			case QJsonValue::Null:
				rs.type = Type::Null;
				rs._obj_ = new QJsonDocument();
				break;
			default:
				break;
			}
		}

		int getDecimalCount(double data) {
			data = qAbs(data);
			data -= (int)data;
			int ct = 0;
			double minValue = 0.0000000001;
			while (!(qAbs(data - 1) < minValue || qAbs(data) < minValue) && ct < 11) {
				data *= 10;
				data -= (int)data;
				ct++;
				minValue *= 10;
			}
			return ct;
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
		jPtr = getJsonProcFunc();
		if (jPtr) {

			jPtr->addValueJson(this->_obj_, name, value.toString());
			return true;
		}
		else {
			return false;
		}
	}

}