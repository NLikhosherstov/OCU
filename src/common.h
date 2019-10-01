#ifndef COMMON_H
#define COMMON_H

#include <math.h>

namespace utils
{
	///
	template<typename Value>
	inline bool isNum(const Value & value)
	{
		return (isnan(value) == false) && (isinf(value) == false);
	}
}

#endif // COMMON_H
