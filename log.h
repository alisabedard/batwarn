// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#ifndef LOG_H
#define LOG_H

#ifdef STDIO
#define ERROR(code, msg) {perror(msg); exit(code);}
#else//!STDIO
#define ERROR(code, msg) exit(code)
#endif//STDIO

#if defined(DEBUG) && defined(STDIO)
#define LOG(...) {fprintf(stderr, __VA_ARGS__); fprintf(stderr, " (%s:%d)\n",\
	__FILE__, __LINE__);}
#define MARK() LOG("MARK")
#else // !DEBUG || !STDIO 
#define LOG(...)
#define MARK()
#endif // DEBUG && STDIO

#define UNUSED(x)

#endif//LOG_H
