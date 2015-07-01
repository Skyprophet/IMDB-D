#ifndef __COMMON_RANDOM_GENERATOR_H__
#define __COMMON_RANDOM_GENERATOR_H__

#include <random>

namespace IMDB {
	namespace Benchmark {	
		class RandomGenerator {
		public:
			static int generateInteger(const int& min, const int& max);
			static int generateDouble(const double& min, const double& max);
			static long generateLong(const long& min, const long& max);

			static std::string generateCString(const int& min_length, const int& max_length);
			static std::string generateNString(const int& min_length, const int& max_length);
			static std::string generateRandomString(const int& min_length, const int& max_length, const char& base, const int& num_characters);
		};
	}
}

#endif
