#ifndef __DISTRIBUTIONS_INTEGER_GENERATOR_H__
#define __DISTRIBUTIONS_INTEGER_GENERATOR_H__

#include "Generator.h"

namespace IMDB {
	namespace Benchmark {
		class IntegerGenerator : public Generator {
		public:
			virtual ~IntegerGenerator() {}

			virtual int nextInt() = 0;
			
			std::string nextString() {
				return std::to_string(nextInt());
			}
			
			std::string lastString() {
				return std::to_string(lastInt());
			}

			int lastInt() {
				return lastint;
			}

		protected:
			void setLastInt(int last) {
				lastint = last;
			}
		private:
			int lastint;
		};
	}
}

#endif
