// util.h
#ifndef _UTIL_H
#define _UTIL_H

/**
 * A debug macro that prints out file, line number, then message.
 * @param  msg The debug message to print.
 */
#define DEBUG(msg) printf("[DEBUG] %s:%d; %s\n", __FILE__, __LINE__, msg)

#endif