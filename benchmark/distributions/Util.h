#ifndef __DISTRIBUTIONS_UTIL_H__
#define __DISTRIBUTIONS_UTIL_H__

#include <string>
#include <cstdlib>


namespace IMDB {
	namespace Benchmark {
		class Utils {
		public:
			static long hash(long val);
			static int FNVhash32(int val);
			static long FNVhash64(long val);
			
		private:
			static const int FNV_offset_basis_32;
			static const int FNV_prime_32;
			static const long FNV_offset_basis_64;
			static const long FNV_prime_64;
		};
	}
}

#endif
