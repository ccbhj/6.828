#ifndef JOS_INC_LOG_H
#define JOS_INC_LOG_H

#define LOG_LEVEL LEVEL_INFO

#define LEVEL_DEBUG 0
#define LEVEL_INFO 1
#define LEVEL_ERROR 2

#define _LOG(level, fmt, ...) \
	do { \
		if ((level) < LOG_LEVEL)  \
			break; \
		cprintf("[%s:%d]"fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
	} while(0)

#define DEBUG(fmt, ...) \
		_LOG(LEVEL_DEBUG, "\x1B[32m[DEBUG] " fmt "\x1B[0m", ##__VA_ARGS__)

#define INFO(fmt, ...) \
		_LOG(LEVEL_INFO, "\x1B[33m[INFO] " fmt "\x1B[0m", ##__VA_ARGS__)

#define ERR(fmt, ...) \
		_LOG(LEVEL_ERROR, "\x1B[31m[ERROR] " fmt "\x1B[0m", ##__VA_ARGS__)


#endif

