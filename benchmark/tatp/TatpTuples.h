#ifndef __TATP_TUPLES_H__
#define __TATP_TUPLES_H__

#include "../BenchmarkTuple.h"
#include "TatpRecords.h"
#include "TatpConstants.h"

namespace IMDB {
	namespace Benchmark {
		namespace TATP {
			TUPLE_CLASS(Subscriber)
			TUPLE_CLASS(AccessInfo)
			TUPLE_CLASS(SpecialFacility)
			TUPLE_CLASS(CallForwarding)

			class DeleteCallForwardingParam : public EventTuple {
			public:
				DeleteCallForwardingParam() {}
				virtual ~DeleteCallForwardingParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				std::string sub_nbr;
				int8_t sf_type;
				int8_t start_time;
			};

			class GetAccessDataParam : public EventTuple {
			public:
				GetAccessDataParam() {}
				virtual ~ GetAccessDataParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				int s_id;
				int8_t ai_type;
			};

			class GetNewDestinationParam : public EventTuple {
			public:
				GetNewDestinationParam() {}
				virtual ~GetNewDestinationParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				int s_id;
				int8_t sf_type;
				int8_t start_time;
				int8_t end_time;
			};

			class GetSubscriberDataParam : public EventTuple {
			public:
				GetSubscriberDataParam() {}
				virtual ~GetSubscriberDataParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				int s_id;
			};

			class InsertCallForwardingParam : public EventTuple {
			public:
				InsertCallForwardingParam() {}
				virtual ~InsertCallForwardingParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				std::string sub_nbr;
				int8_t sf_type;
				int8_t start_time;
				int8_t end_time;
				std::string numberx;
			};

			class UpdateLocationParam : public EventTuple {
			public:
				UpdateLocationParam() {}
				virtual ~UpdateLocationParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				int vlr_location;
				std::string sub_nbr;
			};

			class UpdateSubscriberDataParam : public EventTuple {
			public:
				UpdateSubscriberDataParam() {}
				virtual ~UpdateSubscriberDataParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				int s_id;
				int8_t bit_1;
				int16_t data_a;
				int8_t sf_type;
			};
		}
	}
}

#endif