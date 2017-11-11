#ifndef _LIR_DEBUG_
#define _LIR_DEBUG_

#include "stdio.h"


namespace lir{
	
void log(const char* format)
{
	printf(format);
}

template<typename  T> void log(const char* format, T& t) {
	printf(format, t);
}

template<typename  T,typename Q> void log(const char* format, T& t,Q& q) {
	printf(format, t,q);
}

template<typename  T, typename Q, typename S> void log(const char* format, T& t, Q& q,S& s) {
	printf(format, t, q,s);
}

}
#endif