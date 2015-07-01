#ifndef __EVENT_TUPLE_H__
#define __EVENT_TUPLE_H__

#include <cstdint>
#include <vector>
#include "CharArray.h"

namespace IMDB {
	class EventTuple {
	public:
		EventTuple() : tuple_type(0) {}
		EventTuple(uint8_t tuple_type) : tuple_type(tuple_type) {}
		virtual ~EventTuple() {}
		virtual int64_t getHashCode() const = 0;
		virtual void serialize(CharArray& serial_str) const = 0;
		virtual void deserialize(const CharArray& serial_str) = 0;
	public:
		size_t txn_no = -1;
		uint8_t tuple_type;
	};

	typedef std::vector<EventTuple> EventTuples;
}

#endif
