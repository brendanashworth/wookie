// util.h
// Needs -Ddebug flag to be passed
#ifndef _UTIL_H
#ifdef debug

#define _UTIL_H

 /**
  * A debug macro that prints out file, line number, then message.
  * @param  msg The debug message to print.
  */
 #define DEBUG(msg) printf("[DEBUG] %s:%d; %s\n", __FILE__, __LINE__, msg)

#else

 #define DEBUG(msg) // do nothing if no -Ddebug

#endif
#endif