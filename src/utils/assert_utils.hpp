#ifndef ASSERT_UTILS_HPP
#define ASSERT_UTILS_HPP

#include <cstdlib>
#include <iostream>

#ifndef NDEBUG

#define GEOM_ASSERT(condition)                                                    \
	do                                                                              \
	{                                                                               \
		if (!(condition))                                                             \
		{                                                                             \
			std::cerr << "GEOM_ASSERT failed: " << #condition << "\nFile: " << __FILE__ \
								<< "\nLine: " << __LINE__ << std::endl;                           \
			std::abort();                                                               \
		}                                                                             \
	} while (false)

#else

#define GEOM_ASSERT(condition) ((void)0)

#endif

#ifndef NDEBUG

#define GEOM_DEBUG_WARN(condition, message)                                       \
	do                                                                              \
	{                                                                               \
		if (!(condition))                                                             \
		{                                                                             \
			std::cerr << "GEOM WARNING: " << message << "\nCondition: " << #condition   \
								<< "\nFile: " << __FILE__ << "\nLine: " << __LINE__ << std::endl; \
		}                                                                             \
	} while (false)

#else

#define GEOM_DEBUG_WARN(condition, message) ((void)0)

#endif

#endif // ASSERT_UTILS_HPP