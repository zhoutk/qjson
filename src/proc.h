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
}