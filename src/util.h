// util.h
// various macros, some debug, others not
#ifndef _UTIL_H
#define _UTIL_H

 /**
  * Kills the program and gives some debug info.
  * @param  msg      Message to kill with
  * @param  __LINE__ [description]
  * @return          [description]
  */
 #define DIE(msg, code) {											 \
  printf("CRITICAL ERROR: occurred at %s:%d\n", __FILE__, __LINE__); \
  printf("[message] %s\n", msg);									 \
  printf("[exit code] %d\n", code);									 \
  printf("  Please report this error to the developers.");			 \
  exit(code);														 \
 }

// Debug messages
#ifdef debug

 /**
  * A debug macro that prints out file, line number, then message.
  * @param  msg The debug message to print.
  */
 #define DEBUG(msg) printf("[DEBUG] %s:%d; %s\n", __FILE__, __LINE__, msg)

#else

 #define DEBUG(msg) // do nothing if no -Ddebug

#endif // debug
#endif