#ifndef __DISTRIBUTIONS_SCRAMBLED_ZIPFIAN_GENERATOR_H__
#define __DISTRIBUTIONS_SCRAMBLED_ZIPFIAN_GENERATOR_H__

#include "ZipfianGenerator.h"
#include "Util.h"

namespace IMDB {
	namespace Benchmark {
		class ScrambledZipfianGenerator : public IntegerGenerator {
		public:
			ScrambledZipfianGenerator(long items) : ScrambledZipfianGenerator(0, items - 1) {}
			ScrambledZipfianGenerator(long min, long max) : ScrambledZipfianGenerator(min, max, ZipfianGenerator::ZIPFIAN_CONSTANT) {}
			ScrambledZipfianGenerator(long min, long max, double zipfian_constant);
			virtual ~ScrambledZipfianGenerator() {
				delete gen;
			}

			int nextInt() {
				return (int)nextLong();
			}
			int nextLong();
			double mean();

			static const double ZETAN;
			static const double USED_ZIPFIAN_CONSTANT;
			static const long ITEM_COUNT;
		private:
			ZipfianGenerator* gen;
			long min, max, item_count;

		};
	}
}

#endif
