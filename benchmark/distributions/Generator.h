#ifndef __DISTRIBUTIONS_GENERATOR_H__
#define __DISTRIBUTIONS_GENERATOR_H__

#include <string>

namespace IMDB {
	namespace Benchmark {
		class Generator {
		public:
			virtual ~Generator() {}

			virtual std::string nextString() = 0;
			virtual std::string lastString() = 0;
		};
	}
}

#endif
