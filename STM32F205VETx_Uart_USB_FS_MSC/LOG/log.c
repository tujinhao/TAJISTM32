#include "log.h"
#include <stdio.h>
char* EM_LOGLevelGet(const int level)
{
	if (level == LOG_DEBUG) {
		return "DEBUG";
	}
	else if (level == LOG_INFO) {
		return "INFO";
	}
	else if (level == LOG_WARN) {
		return "WARN";
	}
	else if (level == LOG_ERROR) {
		return "ERROR";
	}
	return "UNLNOW";
}
void EM_LOG(const int level,const char* fun, const int line ,const char* fmt, ...)
{
#ifdef OPEN_LOG
	va_list arg;
	va_start(arg, fmt);
	char buf[50] = { 0 };
	vsnprintf(buf, sizeof(buf), fmt, arg);
	va_end(arg);
	if (level >= LOG_LEVEL)
		
		printf("[%-5s] [%-10s%4d] %s \r\n", EM_LOGLevelGet(level), fun, line, buf);
#endif
}







