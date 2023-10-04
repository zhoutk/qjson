#pragma once

#include <QJsonDocument>
#include <QString>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

namespace QJSON {
	enum class JsonType
	{
		Object = 6,
		Array = 7
	};

	class Json final {
		public:
		template <typename T> bool addSubitem(const QString& name, const T& value)  //object
		{
			if (this->type == Type::Object) {
				QJsonObject json = this->_obj_->object();
				if (json.contains(name))
					json.remove(name);
				json.insert(name, QJsonValue::fromVariant(value));
				delete this->_obj_;
				this->_obj_ = new QJsonDocument(json);
			}
			else 
				return false;
		};

		template<typename T> bool addSubitem(const T& value) {			//Array
			if (this->type == Type::Array) {
				QJsonArray json = this->_obj_->array();
				json.push_back(QJsonValue::fromVariant(value));
				delete this->_obj_;
				this->_obj_ = new QJsonDocument(json);
			}
			else
				return false;
		}

		bool addSubitem(std::initializer_list<Json> values) {
			if (this->type == Type::Array) {
				for (Json al : values)
				{
					this->ExtendItem(al);
				}
				return true;
			}
			else
				return false;
		}

		Json() : Json(JsonType::Object) {}

		Json(JsonType type) : type((Type)type) {
			_obj_ = new QJsonDocument;
		}

		Json(std::nullptr_t) {
			new (this)Json();
			this->type = Type::Null;
		}

		Json(bool data) {
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

		bool contains(const QString& key) {
			Json f = (*this)[key];
			return !(f.type == Type::Null || f.type == Type::Error);
		}

		QString getValueType() {
			return TYPENAMES[this->type];
		}

		Json getAndRemove(const QString& key) {
			Json rs = (*this)[key];
			if ((rs.type != Type::Null && rs.type != Type::Error)) {
				QJsonObject obj = this->_obj_->object();
				obj.remove(key);
				delete this->_obj_;
				this->_obj_ = new QJsonDocument(obj);
			}
			return rs;
		}

		QStringList getAllKeys() {
			QStringList rs;
			if (this->type == Type::Object) {
				QJsonObject obj = this->_obj_->object();
				return obj.keys();
			}
			return rs;
		}

		bool concat(const Json& value) {
			if (this->type == Type::Array) {
				if (value.type == Type::Array) {
					QJsonArray tarr = this->_obj_->array();
					QJsonArray varr = value._obj_->array();
					for (int i = 0; i < varr.size(); i++) 
						tarr.push_back(varr[i]);
					delete this->_obj_;
					this->_obj_ = new QJsonDocument(tarr);
				}
				else if (value.type == Type::Object) {
					QJsonArray tarr = this->_obj_->array();
					QJsonObject obj = value._obj_->object();
					QJsonObject::const_iterator it = obj.constBegin();
					QJsonObject::const_iterator end = obj.constEnd();
					while (it != end)
					{
						tarr.push_back(it.value());
						it++;
					}
					delete this->_obj_;
					this->_obj_ = new QJsonDocument(tarr);
				}
				else {
					this->ExtendItem(value);
				}
				return true;
			}
			else {
				return false;
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
		QStringList TYPENAMES {"Error", "False", "True", "Null", "Number", "String", "Object", "Array"};
		Type type;
		QJsonDocument* _obj_;
		QString vdata;


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

		void ExtendItem(QString name, Json cur);
		void ExtendItem(Json cur);

	};

	template <> bool Json::addSubitem(const QString& name, const std::nullptr_t&) {
		if (this->type == Type::Object) {
			QJsonObject json = this->_obj_->object();
			if (json.contains(name))
				json.remove(name);
			json.insert(name, QJsonValue::Null);
			delete this->_obj_;
			this->_obj_ = new QJsonDocument(json);
		}
		else 
			return false;
	}
	template <> bool Json::addSubitem(const QString& name, const Json& value) {
		if (this->type == Type::Object) {
			QJsonObject json = this->_obj_->object();
			if (json.contains(name))
				json.remove(name);
			QJsonParseError json_error;
			QJsonDocument jsonDocument = QJsonDocument::fromJson(value.toString().toUtf8(), &json_error);
			if (json_error.error == QJsonParseError::NoError) {
				json.insert(name, QJsonValue::fromVariant(QVariant(jsonDocument)));
			}
			delete this->_obj_;
			this->_obj_ = new QJsonDocument(json);
		}
		else 
			return false;
	}
	template <> bool Json::addSubitem(const std::nullptr_t&) {
		if (this->type == Type::Array) {
			QJsonArray json = this->_obj_->array();
			json.push_back(QJsonValue::Null);
			delete this->_obj_;
			this->_obj_ = new QJsonDocument(json);
		}
		else
			return false;
	}
	template <> bool Json::addSubitem(const Json& value) {
		if (this->type == Type::Array) {
			QJsonArray json = this->_obj_->array();
			QJsonParseError json_error;
			QJsonDocument jsonDocument = QJsonDocument::fromJson(value.toString().toUtf8(), &json_error);
			if (json_error.error == QJsonParseError::NoError) {
				json.push_back(QJsonValue::fromVariant(QVariant(jsonDocument)));
			}
			delete this->_obj_;
			this->_obj_ = new QJsonDocument(json);
		}
		else
			return false;
	}

	void Json::ExtendItem(QString name, Json cur) {			//Object
		switch (cur.type)
		{
		case Type::False:
			this->addSubitem(name, false);
			break;
		case Type::True:
			this->addSubitem(name, true);
			break;
		case Type::Null:
			this->addSubitem(name, nullptr);
			break;
		case Type::Number:
			this->addSubitem(name, cur.vdata.toDouble());
			break;
		case Type::String:
			this->addSubitem(name, cur.vdata);
			break;
		case Type::Object:
		case Type::Array:
			this->addSubitem(name, cur);
			break;
		default:
			break;
		}
	}

	void Json::ExtendItem(Json cur) {				//Array
		switch (cur.type)
		{
		case Type::False:
			this->addSubitem(false);
			break;
		case Type::True:
			this->addSubitem(true);
			break;
		case Type::Null:
			this->addSubitem(nullptr);
			break;
		case Type::Number:
			this->addSubitem(cur.vdata.toDouble());
			break;
		case Type::String:
			this->addSubitem(cur.vdata);
			break;
		case Type::Object:
		case Type::Array:
			this->addSubitem(cur);
			break;
		default:
			break;
		}
	}

}