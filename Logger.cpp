#include "Logger.h"
#include <debugapi.h>

namespace Logger {


	//log�쐬
	void Log(const std::string& message) {
		OutputDebugStringA(message.c_str());

	}

}
