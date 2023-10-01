#include "qjson.h"

namespace QJSON {
	Json::ObjectProc* Json::ObjectProc::appPtr = nullptr;
	Json::ArrayProc* Json::ArrayProc::appPtr = nullptr;
}