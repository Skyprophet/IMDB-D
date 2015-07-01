#ifndef __BENCHMARK_TUPLE_H__
#define __BENCHMARK_TUPLE_H__

#include "../common/EventTuple.h"

namespace IMDB {
	namespace Benchmark {

#define TUPLE_CLASS(TupleName) \
		class TupleName##Tuple : public EventTuple{ \
		public: \
			TupleName##Tuple() { \
				memset(reinterpret_cast<char*>(&data_), 0, sizeof(data_)); \
			} \
			virtual ~TupleName##Tuple(){} \
			virtual int64_t GetHashCode() const { \
				return -1; \
			} \
			virtual void Serialize(CharArray& serial_str) const { \
				serial_str.allocate(sizeof(TupleName##Record)); \
				serial_str.Memcpy(0, reinterpret_cast<const char*>(&data_), sizeof(TupleName##Record)); \
			} \
			virtual void Deserialize(const CharArray& serial_str) { \
				memcpy(&data_, serial_str.data, serial_str.size); \
			} \
		public: \
			TupleName##Record data_; \
		};
	}
}

#endif