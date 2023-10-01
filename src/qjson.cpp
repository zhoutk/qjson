#include "qjson.h"

namespace QJSON {
	ObjectProc* ObjectProc::appPtr = nullptr;
	ArrayProc* ArrayProc::appPtr = nullptr;

	ObjectProc* Json::oPtr = ObjectProc::Instance();
	ArrayProc* Json::aPtr = ArrayProc::Instance();
}