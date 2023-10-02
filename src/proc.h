#pragma once
//#pragma execution_character_set("utf-8")

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

	class JsonProc {
		public:
		virtual void appendValue(QJsonDocument*& obj, const QString& name, const QVariant* value) = 0;
	};

	class ObjectProc : public JsonProc {
		public:
		static ObjectProc* Instance(){
			if(appPtr == nullptr)
				appPtr = new ObjectProc;
			return appPtr;
			
		}

		void appendValue(QJsonDocument*& obj, const QString& name, const QVariant* value){
			QJsonObject json = obj->object();
			if (json.contains(name))
				json.remove(name);
			if (value == nullptr)
				json.insert(name, QJsonValue::Null);
			else {
				std::string dd = value->toString().toStdString();
				json.insert(name, QJsonValue::fromVariant(*value));
			}
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
	ObjectProc* ObjectProc::appPtr = nullptr;

	class ArrayProc : public JsonProc {
		public:
		static ArrayProc* Instance(){
			if(appPtr == nullptr)
				appPtr = new ArrayProc;
			return appPtr;
			
		}

		void appendValue(QJsonDocument*& obj, const QString& name, const QVariant* value){
			QJsonArray json = obj->array();
			if (value == nullptr)
				json.push_back(QJsonValue::Null);
			else
				json.push_back(QJsonValue::fromVariant(*value));
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
	ArrayProc* ArrayProc::appPtr = nullptr;
}