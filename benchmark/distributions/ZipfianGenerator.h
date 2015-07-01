#ifndef __DISTRIBUTIONS_ZIPFIAN_GENERATOR_H__
#define __DISTRIBUTIONS_ZIPFIAN_GENERATOR_H__

#include "IntegerGenerator.h"
#include "RandomGenerator.h"

#include <cmath>
#include <mutex>
#include <random>

namespace IMDB {
	namespace Benchmark {
		class ZipfianGenerator : public IntegerGenerator {
			public:	
				ZipfianGenerator(long items): ZipfianGenerator(0, items - 1) {}
				ZipfianGenerator(long min, long max) : ZipfianGenerator(min, max, ZIPFIAN_CONSTANT) {}
				ZipfianGenerator(long items, double zipfian_constant) : ZipfianGenerator(0, items - 1, zipfian_constant) {}
				ZipfianGenerator(long min, long max, double zipfian_constant) : ZipfianGenerator(min, max, zipfian_constant, zetastatic(max - min + 1, zipfian_constant)) {}
				ZipfianGenerator(long min, long max, double zipfian_constant, double zetan);
				virtual ~ZipfianGenerator() {}

				int nextInt(int item_count) {
					return (int)nextLong(item_count);
				}

				long nextLong(long item_count);

				int nextInt() {
					return (int)nextLong(items);
				}

				long nextLong() {
					return nextLong(items);
				}
	
				static double zetastatic(long st, long n, double theta, double initialsum);

				static double zetastatic(long n, double theta);

		
			private:
				double zeta(long n, double zeta) {
					count_for_zeta = n;
					return zetastatic(n, theta);
				}

				double zeta(long st, long n, double theta, double initialsum) {
					count_for_zeta = n;
					return zetastatic(st, n, theta, initialsum);
				}

			public:		
				static const double ZIPFIAN_CONSTANT;
			
			private:
				long items;
				long base;
				double zipfian_constant;
				double alpha, zetan, eta, theta, zeta2theta;
				long count_for_zeta;
				bool allow_item_count_decrease = false;
				
				std::mutex recompute_mutex;
		};
	}
}

#endif
