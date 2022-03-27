
#ifndef __LOG_H_
#define __LOG_H_
#include <stdarg.h>
#define OPEN_LOG 0
#define LOG_LEVEL LOG_DEBUG

typedef enum
{
	LOG_DEBUG = 0,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
}E_LOGLEVEL;
void EM_LOG(const int level, const char* fun, const int line, const char* fmt, ...);
#define EMLOG(level,fmt,...) EM_LOG(level,__FUNCTION__,__LINE__,fmt, ##__VA_ARGS__)




#endif
