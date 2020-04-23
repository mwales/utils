#pragma once

#include <string>

void registrationHandler(std::string name);

#define FORCE_EXECUTE(className) \
	class className { \
		public: \
			className(); \
	}; \
	static className className_instance; \
	className::className() \

