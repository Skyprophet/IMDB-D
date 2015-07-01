#ifndef __META_TYPES_H__
#define __META_TYPES_H__

#include <cstdint>
#include <string>
#include <cassert>

namespace IMDB {
	namespace StorageEngine {
		const size_t ENDEXECUTION = 100;

		const size_t kBlockSize = 8 * 1024 * 1024;
		const size_t kRecordHeaderSize = sizeof(size_t);
		const size_t kFetchSize = 32;

		enum ValueType {INT, INT8, INT16, INT32, INT64, DOUBLE, FLOAT, VARCHAR};
		enum IndexType {HASHMAP, BTREE};
		enum RecordFlag {FREE, OCCUPIED};

		const size_t kIntSize = sizeof(int);
		const size_t kInt8Size = sizeof(int8_t);
		const size_t kInt16Size = sizeof(int16_t);
		const size_t kInt32Size = sizeof(int32_t);
		const size_t kInt64Size = sizeof(int64_t);
		const size_t kFloatSize = sizeof(float);
		const size_t kDoubleSize = sizeof(double);

		struct ColumnInfo {
			ColumnInfo(const std::string& column_name, const ValueType& column_type, bool is_primary = false)
				: column_name(column_name), column_type(column_type), column_offset(0), is_primary(is_primary) {
				assert(column_type != ValueType::VARCHAR);
				switch (column_type) {
				case INT:
					column_size = kIntSize;
					break;
				case INT8:
					column_size = kInt8Size;
					break;
				case INT16:
					column_size = kInt16Size;
					break;
				case INT32:
					column_size = kInt32Size;
					break;
				case INT64:
					column_size = kInt64Size;
					break;
				case FLOAT:
					column_size = kFloatSize;
					break;
				case DOUBLE:
					column_size = kDoubleSize;
					break;
				default:
					assert(false);
				}
			}

			ColumnInfo(const std::string& column_name, const ValueType& column_type, const size_t& column_size, bool is_primary = false)
				: column_name(column_name), column_type(column_type), column_size(column_size), column_offset(0), is_primary(is_primary)
			{}

			const std::string column_name;
			const ValueType column_type;
			size_t column_size;
			size_t column_offset;
			bool is_primary;
		};

		struct IndexInfo {
			IndexInfo(const IndexType& index_type, const size_t* column_ids, const size_t column_num) :index_type(index_type) {
				this->column_ids = new size_t[column_num];
				for (size_t i = 0; i < column_num; ++i) {
					this->column_ids[i] = column_ids[i];
				}
				this->column_num = column_num;
			}

			~IndexInfo() {
				delete[] column_ids;
			}

			std::string getSerializedSymbol() const {
				return std::string(reinterpret_cast<char*>(column_ids), sizeof(size_t) * column_num);
			}

			const IndexType index_type;
			size_t* column_ids;
			size_t column_num;
		};

		struct ColumnPredicate {
			ColumnPredicate(const size_t& column_id, const char* const value) : column_id(column_id), value(value) {}
			const size_t column_id;
			const char* const value;
		};

		struct ColumnPredicates {
			ColumnPredicates(const size_t* column_ids, const char* const * const column_values, const size_t& column_num) : column_ids(column_ids), column_values(column_values), column_num(column_num) {}
			const size_t* column_ids;
			const char* const *const column_values;
			const size_t column_num;
		};
	}
}

#endif
