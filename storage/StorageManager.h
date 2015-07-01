#ifndef __STORAGE_MANAGER_H__
#define __STORAGE_MANAGER_H__

#include "SchemaTable.h"
#include <sstream>

namespace IMDB {
	namespace StorageEngine {
		class StorageManager {
		public:
			StorageManager() {}
			~StorageManager() {}
		
			void registerTable(const size_t& table_id, const Schema* const table_schema) {
				schema_tables[table_id] = new SchemaTable(table_schema);
			}

			void registerTable(const size_t& table_id, const Schema* const table_schema, const IndexInfo* indexes, const size_t& index_num) {
				schema_tables[table_id] = new SchemaTable(table_schema, indexes, index_num);
			}

			void dropAllTables() {
				for (auto & entry : schema_tables)
					delete entry.second;
				schema_tables.clear();
			}

			const size_t& getTableSize(const size_t& table_id) const {
				return schema_tables.at(table_id)->getTableSize();
			}

			SchemaTableIterator *getSchemaTableIterator(const size_t& table_id) const {
				return schema_tables.at(table_id)->getSchemaTableIterator();
			}

			void insertRecord(const size_t& table_id, const char* data) {
				schema_tables.at(table_id)->insertRecord(data);
			}

			void insertRecord(const size_t& table_id, const Record& record) {
				schema_tables.at(table_id)->insertRecord(record);
			}

			void deleteRecord(const size_t& table_id, const ConstRecord& record) {
				schema_tables.at(table_id)->deleteRecord(record);
			}

			void updateRecord(const size_t& table_id, const ConstRecord& record, const ColumnPredicate& param) {
				schema_tables.at(table_id)->updateRecord(record, param);
			}

			void updateRecord(const size_t& table_id, const ConstRecord& record, const ColumnPredicates& params) {
				schema_tables.at(table_id)->updateRecord(record, params);
			}

			void updateRecord(const size_t& table_id, const ConstRecords& records, const size_t& record_id, const ColumnPredicate& param) {
				schema_tables.at(table_id)->updateRecord(records, record_id, param);
			}

			void updateRecord(const size_t& table_id, const ConstRecords& records, const size_t& record_id, const ColumnPredicates& params) {
				schema_tables.at(table_id)->updateRecord(records, record_id, params);
			}

			ConstRecord selectRecord(const size_t& table_id, const std::string& primary_key) {
				return schema_tables.at(table_id)->selectRecord(primary_key);
			}

			ConstRecord selectRecord(const size_t& table_id, const ColumnPredicate& param) {
				return schema_tables.at(table_id)->selectRecord(param);
			}

			ConstRecords selectRecords(const size_t& table_id, const ColumnPredicate& param) {
				return schema_tables.at(table_id)->selectRecords(param);
			}
			
			ConstRecord selectRecord(const size_t& table_id, const ColumnPredicates& params) {
				return schema_tables.at(table_id)->selectRecord(params);
			}

 			ConstRecords selectRecords(const size_t& table_id, const ColumnPredicates& params) {
				return schema_tables.at(table_id)->selectRecords(params);
			}

			std::string getStatisticsString() {
				std::stringstream ss;
				uint64_t database_size = 0;
				for (auto & entry : schema_tables) {
					ss << "table_id = " << entry.first << " : " << entry.second->getTableSize() << std::endl;
					database_size += entry.second->getRecordSize() * entry.second->getTableSize();
				}
				ss << "database size = " << database_size * 1.0 / 1024 / 1024 << " MB" << std::endl;
				return ss.str();
			}
		private:
			StorageManager(const StorageManager&);
			StorageManager& operator=(const StorageManager&);

			std::unordered_map<size_t, SchemaTable*> schema_tables;
		};
	}
}

#endif
