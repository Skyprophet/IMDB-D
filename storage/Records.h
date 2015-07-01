#ifndef __RECORDS_H__
#define __RECORDS_H__

#include "Record.h"

namespace IMDB {
	namespace StorageEngine {
		class ConstRecords;

		class Records {
			friend class ConstRecords;
		public:
			Records(const Schema* const schema_ptr, const size_t& init_size)
				: schema_ptr(schema_ptr), occupied_record_num(0), allocated_record_num(init_size) {
				data_ptrs = new char*[init_size];
			}
			Records(const Schema* const schema_ptr, char** data_ptrs, const size_t& record_num)
				: schema_ptr(schema_ptr), occupied_record_num(record_num), allocated_record_num(record_num), data_ptrs(data_ptrs) {}
			Records(const Records& records)
				: schema_ptr(records.schema_ptr), occupied_record_num(records.occupied_record_num), allocated_record_num(records.allocated_record_num) {
				data_ptrs = new char* [allocated_record_num];
				memcpy(data_ptrs, records.data_ptrs, sizeof(char*) * allocated_record_num);
			}

			~Records() {
				delete[] data_ptrs;
			}

			const size_t& getRecordNum() const {
				return occupied_record_num;
			}

			void insertRecord(const Record& record) {
				data_ptrs[occupied_record_num] = record.fetchData();
				++occupied_record_num;
				if (occupied_record_num == allocated_record_num) {
					char** tmp_data_ptrs = data_ptrs;
					data_ptrs = new char*[allocated_record_num * 2];
					memcpy(data_ptrs, tmp_data_ptrs, sizeof(char*) * allocated_record_num);
					allocated_record_num *= 2;
					delete[] tmp_data_ptrs;
				}
			}

			const Schema* const getSchema() const {
				return schema_ptr;
			}

			const size_t& getColumnSize(const size_t& column_id) const  {
				return schema_ptr->getColumnSize(column_id);
			}

			const size_t& getRecordSize() const {
				return schema_ptr->getSchemaSize();
			}

			const size_t& getColumnCount() const {
				return schema_ptr->getColumnCount();
			}
		private:
			Records& operator=(const Records& record);

			const Schema* const schema_ptr;
			size_t occupied_record_num;
			size_t allocated_record_num;
			char** data_ptrs;
		};


		class ConstRecords {
		public:
			ConstRecords(const Records& records) : schema_ptr(records.schema_ptr), record_num(records.occupied_record_num) {
				char** tmp_data_ptrs = new char*[record_num];
				memcpy(tmp_data_ptrs, records.data_ptrs, sizeof(char*) * record_num);
				data_ptrs = tmp_data_ptrs;
			}

			ConstRecords(const ConstRecords& records) : schema_ptr(records.schema_ptr), record_num(records.record_num) {
				char** tmp_data_ptrs = new char*[record_num];
				memcpy(tmp_data_ptrs, records.data_ptrs, sizeof(char*) * record_num);
				data_ptrs = tmp_data_ptrs;
			}

			~ConstRecords() {
				delete[] data_ptrs;
			}

			size_t getRecordNum() const {
				return record_num;
			}

			ConstRecord getRecord(const size_t& record_id) const {
				return ConstRecord(schema_ptr, data_ptrs[record_id]);
			}

			const char* getColumn(const size_t& record_id, const size_t& column_id) const {
				return data_ptrs[record_id] + schema_ptr->getColumnOffset(column_id);
			}
		private:
			Records& operator=(const Records& record);

			const Schema* const schema_ptr;
			size_t record_num;
			char** data_ptrs;
		};
	}
}

#endif
