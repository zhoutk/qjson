#include "qjson.h"

namespace QJSON {
	Json::ObjectProc* Json::ObjectProc::appPtr = nullptr;
	Json::ArrayProc* Json::ArrayProc::appPtr = nullptr;

	Json::ObjectProc* Json::oPtr = Json::ObjectProc::Instance();
	Json::ArrayProc* Json::aPtr = Json::ArrayProc::Instance();
}