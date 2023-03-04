#include "ifs_time.h"
#include <iomanip>

#include <ctime>
#include <sstream>

namespace ifs {
	std::string timestamp(std::string fmt) {

		// Source: https://stackoverflow.com/questions/35258285/how-to-use-localtime-s-with-a-pointer-in-c
		// (localtime produces deprecation error in VS. Read the discussion on stackoverflow page above)
		struct tm newtime;
		time_t now = time(0);
		localtime_s(&newtime, &now);
		std::ostringstream oss;
		oss << std::put_time(&newtime, fmt.c_str());
		auto timeStamp = oss.str();
		return timeStamp;
	}
}