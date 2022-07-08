#ifndef JOS_KERN_KDEBUG_H
#define JOS_KERN_KDEBUG_H

#include <inc/types.h>

// Debug information about a particular instruction pointer
struct Eipdebuginfo {
	const char *eip_file;		// Source code filename for EIP
	int eip_line;			// Source code linenumber for EIP

	const char *eip_fn_name;	// Name of function containing EIP
					//  - Note: not null terminated!
	int eip_fn_namelen;		// Length of function name
	uintptr_t eip_fn_addr;		// Address of start of function
	int eip_fn_narg;		// Number of function arguments
};

int debuginfo_eip(uintptr_t eip, struct Eipdebuginfo *info);


#define LOG_LEVEL LEVEL_DEBUG

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
		_LOG(LEVEL_DEBUG, "[DEBUG] " fmt, ##__VA_ARGS__)

#define INFO(fmt, ...) \
		_LOG(LEVEL_INFO, "[INFO] " fmt, ##__VA_ARGS__)

#define ERROR(fmt, ...) \
		_LOG(LEVEL_ERROR, "[ERROR] " fmt, ##__VA_ARGS__)

#endif
