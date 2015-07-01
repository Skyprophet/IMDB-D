#ifndef __SCHEMA_TABLE_H__
#define __SCHEMA_TABLE_H__

#include <cassert>
#include <cstring>
#include <unordered_map>

#include "Schema.h"
#include "Record.h"
#include "Records.h"
#include "BaseTable.h"
#include "BaseUniqueIndex.h"
#include "BaseMultiIndex.h"
#include "SchemaTableIterator.h"
#include "MetaTypes.h"
#include "TBBMultiIndex.h"
#include "TBBUniqueIndex.h"

namespace IMDB {
	namespace StorageEngine {
		class SchemaTable : public BaseTable {
		public:
			SchemaTable(const Schema* const schema_ptr);
			SchemaTable(const Schema* const schema_ptr, const IndexInfo* const indexes, const size_t& index_num);
			virtual ~SchemaTable();

			SchemaTableIterator* getSchemaTableIterator() const;
			const size_t& getRecordSize() const;

			void insertRecord(const char* data);
			void insertRecord(const Record& record);
			
			void deleteRecord(const ConstRecord& record);
			
			//TODO update indexes for update operation
			void updateRecord(const ConstRecord& record, const ColumnPredicate& param);
			void updateRecord(const ConstRecord& record, const ColumnPredicates& params);
			void updateRecord(const ConstRecords& records, const size_t& record_id, const ColumnPredicate& param);
			void updateRecord(const ConstRecords& records, const size_t& record_id, const ColumnPredicates& params);

			Record selectRecord(const std::string& primary_key) const;
			Record selectRecord(const ColumnPredicate& param) const;
			Record selectRecord(const ColumnPredicates& params) const;
			Records selectRecords(const ColumnPredicate& param) const;
			Records selectRecords(const ColumnPredicates& params) const;
		private:
			SchemaTable(const SchemaTable&);
			SchemaTable& operator=(const SchemaTable&);

			const Schema* const schema_ptr;
			const size_t column_count;
			std::unordered_map<std::string, int> column_names;
			std::pair<std::string, BaseUniqueIndex*> primary_index_ptr;
			std::unordered_map<std::string, BaseMultiIndex*> secondary_index_ptrs;
		};
	}
}

#endif
