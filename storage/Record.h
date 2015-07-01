#ifndef __RECORD_H__
#define __RECORD_H__

#include "../common/CharArray.h"
#include "Schema.h"
#include "MetaTypes.h"

#include <cassert>
#include <string>

namespace IMDB {
	namespace StorageEngine {
		class ConstRecord;

		class Record {
			friend ConstRecord;
		public:
			Record(const Schema* const schema_ptr) : schema_ptr(schema_ptr), data_ptr(NULL) {}
			Record(const Schema* const schema_ptr, char* data_ptr) : schema_ptr(schema_ptr), data_ptr(data_ptr) {}
			~Record() {}

			//soft copy
			void transferData(char* data_ptr);
			char* fetchData() const;
			const Schema* getSchema() const;
			
			void setColumn(const size_t& column_id, const char* data);
			void setColumn(const size_t& column_id, const CharArray& data);
			void setColumn(const size_t& column_id, const char* data_str, const size_t& data_size);
			void setColumn(const size_t& column_id, const std::string& data);

			char* getColumn(const size_t& column_id) const;
			char* getColumn(const size_t& column_id, char* data) const;
			CharArray& getColumn(const size_t& column_id, CharArray& data) const;
			void getColumn(const size_t& column_id, std::string& data) const;

			const size_t& getColumnSize(const size_t& column_id) const;
			const size_t& getRecordSize() const;
			const size_t& getColumnCount() const;

			std::string getPrimaryKeySymbol() const;
			std::string getKeySymbol(const size_t* index_cols, const size_t& index_col_num) const;
		private:
			Record& operator=(const Record&);

			const Schema* const schema_ptr;
			char* data_ptr;
		};

		class ConstRecord {
			public:
				ConstRecord(const Schema* const schema_ptr) : schema_ptr(schema_ptr), data_ptr(NULL) {}
				ConstRecord(const Schema* const schema_ptr, const char* const data_ptr) : schema_ptr(schema_ptr), data_ptr(data_ptr) {}
				ConstRecord(const Record& record) : schema_ptr(record.schema_ptr), data_ptr(record.data_ptr) {}

				const char* fetchData() const;
				const char* getColumn(const size_t& column_id) const;
				char* getColumn(const size_t& column_id, char* data) const;
				CharArray& getColumn(const size_t& column_id, CharArray& data) const;
				void getColumn(const size_t& column_id, std::string& data) const;
				
				const size_t& getColumnSize(const size_t& column_id) const;
				const size_t& getRecordSize() const;
				const size_t& getColumnCount() const;
				
				std::string getPrimaryKeySymbol() const;
				std::string getKeySymbol(const size_t* index_cols, const size_t& index_col_num) const;
			private:
				ConstRecord& operator=(const ConstRecord&);

				const Schema* const schema_ptr;
				const char* const data_ptr;
		};
	}
}

#endif
