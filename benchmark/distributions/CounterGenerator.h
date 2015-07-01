#ifndef __COUNTER_GENERATOR_H__
#define __COUNTER_GENERATOR_H__

#include "IntegerGenerator.h"

#include <atomic>

namespace IMDB {
	namespace Benchmark {
		class CounterGenerator : public IntegerGenerator {
		public:
			CounterGenerator(int count_start) {
				counter.store(count_start);
				setLastInt(counter.load() - 1);
			}

			int nextInt() {
				int ret = counter.fetch_add(1);
				setLastInt(ret);
				return ret;
			}

			int lastInt() {
				return counter.load() - 1;
			}

			void reset() {
				counter.store(0);
				setLastInt(counter.load() - 1);
			}
		private:
			std::atomic_int counter;
		};
	}
}

#endif
