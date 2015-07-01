#include "SchemaTable.h"

using namespace IMDB::StorageEngine;

SchemaTable::SchemaTable(const Schema* const schema_ptr) : BaseTable(schema_ptr->getSchemaSize()), schema_ptr(schema_ptr), column_count(schema_ptr->getColumnCount()) {
	for (size_t i = 0; i < column_count; ++i)
		column_names[schema_ptr->getColumnName(i)] = i;
	if (schema_ptr->getPrimaryColumnCount() != 0)
		primary_index_ptr = std::make_pair(schema_ptr->getPrimaryColumnSymbol(), new TBBUniqueIndex());
}

SchemaTable::SchemaTable(const Schema* const schema_ptr, const IndexInfo* const indexes, const size_t& index_num): SchemaTable(schema_ptr) {
	for (size_t i = 0; i < index_num; ++i)
		secondary_index_ptrs[indexes[i].getSerializedSymbol()] = new TBBMultiIndex();
}

SchemaTable::~SchemaTable() {
	delete primary_index_ptr.second;
	for (auto& entry : secondary_index_ptrs)
		delete entry.second;
}

SchemaTableIterator* SchemaTable::getSchemaTableIterator() const {
	return new SchemaTableIterator(schema_ptr, BaseTable::getIterator());
}

const size_t& SchemaTable::getRecordSize() const {
	return schema_ptr->getSchemaSize();
}

void SchemaTable::insertRecord(const char* data) {
	ConstRecord record(schema_ptr, data);
	char* address = BaseTable::insertRecord(data);

	if (primary_index_ptr.second != NULL)
		primary_index_ptr.second->insertRecord(record.getPrimaryKeySymbol(), address);

	for (auto & entry : secondary_index_ptrs) {
		size_t index_col_num = entry.first.size() / sizeof(size_t);
		size_t* index_cols = new size_t[index_col_num];
		memcpy(index_cols, entry.first.c_str(), entry.first.size());
		entry.second->insertRecord(record.getKeySymbol(index_cols, index_col_num), address);
		delete[] index_cols;
	}
}

void SchemaTable::insertRecord(const Record& record) {
	assert(schema_ptr == record.getSchema());
	char* address = BaseTable::insertRecord(record.fetchData());

	if (primary_index_ptr.second != NULL)
		primary_index_ptr.second->insertRecord(record.getPrimaryKeySymbol(), address);

	for (auto & entry : secondary_index_ptrs) {
		size_t index_col_num = entry.first.size() / sizeof(size_t);
		size_t* index_cols = new size_t[index_col_num];
		memcpy(index_cols, entry.first.c_str(), entry.first.size());
		entry.second->insertRecord(record.getKeySymbol(index_cols, index_col_num), address);
		delete[] index_cols;
	}
}

void SchemaTable::deleteRecord(const ConstRecord& record) {
	BaseTable::deleteRecord(const_cast<char*>(record.fetchData()));

	if (primary_index_ptr.second != NULL)
		primary_index_ptr.second->deleteRecord(record.getPrimaryKeySymbol());

	for (auto & entry : secondary_index_ptrs) {
		size_t index_col_num = entry.first.size() / sizeof(size_t);
		size_t* index_cols = new size_t[index_col_num];
		memcpy(index_cols, entry.first.c_str(), entry.first.size());
		entry.second->deleteRecord(record.getKeySymbol(index_cols, index_col_num));
		delete[] index_cols;
	}
}

void SchemaTable::updateRecord(const ConstRecord& record, const ColumnPredicate& param) {
	char* data_ptr = const_cast<char*>(record.getColumn(param.column_id));
	memcpy(data_ptr, param.value, schema_ptr->getColumnSize(param.column_id));

	//TODO update index here
}

void SchemaTable::updateRecord(const ConstRecord& record, const ColumnPredicates& params) {
	for (size_t i = 0; i < params.column_num; ++i) {
		char* data_ptr = const_cast<char*>(record.getColumn(params.column_ids[i]));
		memcpy(data_ptr, params.column_values[i], schema_ptr->getColumnSize(params.column_ids[i]));
	}
	
	//TODO update index here
}

void SchemaTable::updateRecord(const ConstRecords& records, const size_t& record_id , const ColumnPredicate& param) {
	char* data_ptr = const_cast<char*>(records.getColumn(record_id, param.column_id));
	memcpy(data_ptr, param.value, schema_ptr->getColumnSize(param.column_id));

	//TODO update index here
}

void SchemaTable::updateRecord(const ConstRecords& records, const size_t& record_id, const ColumnPredicates& params) {
	for (size_t i = 0; i < params.column_num; ++i) {
		char* data_ptr = const_cast<char*>(records.getColumn(record_id, params.column_ids[i]));
		memcpy(data_ptr, params.column_values[i], schema_ptr->getColumnSize(params.column_ids[i]));
	}
	
	//TODO update index here
}

Record SchemaTable::selectRecord(const std::string& primary_key) const {
	char* address = primary_index_ptr.second->searchRecord(primary_key);
	if (address != NULL) return Record(schema_ptr, address);
	else return Record(NULL);
}

Record SchemaTable::selectRecord(const ColumnPredicate& param) const {
	//if there exists primary index
	std::string pred_col_symbol(reinterpret_cast<const char*>(&param.column_id), sizeof(size_t));
	if (pred_col_symbol == primary_index_ptr.first) {
		char* address = primary_index_ptr.second->searchRecord(schema_ptr->getPredicateDataSymbol(param));
		if (address != NULL) {
			Record record(schema_ptr);
			record.transferData(address);
			return record;
		} else return Record(NULL);
	}
	
	//if there exists secondary index
	if (secondary_index_ptrs.find(pred_col_symbol) != secondary_index_ptrs.end()) {
		char* address = secondary_index_ptrs.at(pred_col_symbol)->searchRecord(schema_ptr->getPredicateDataSymbol(param));
		if (address != NULL) {
			Record record(schema_ptr);
			record.transferData(address);
			return record;
		} else return Record(NULL);
	}

	//otherwise, need to scan
	Record record(schema_ptr);
	TableIterator* iterator = getIterator();
	while (iterator->hasNext()) {
		record.transferData(iterator->next());
		if (!std::memcmp(param.value, record.getColumn(param.column_id), record.getColumnSize(param.column_id))) {
			delete iterator;
			return record;
		}
	}
	delete iterator;
	return Record(NULL);
}

Records SchemaTable::selectRecords(const ColumnPredicate& param) const {
	std::string sec_col_symbol(reinterpret_cast<const char*>(&param.column_id), sizeof(size_t));
	if (secondary_index_ptrs.find(sec_col_symbol) != secondary_index_ptrs.end()) {
		char** record_ptrs = NULL;
		size_t record_num = 0;
		secondary_index_ptrs.at(sec_col_symbol)->searchRecords(schema_ptr->getPredicateDataSymbol(param), record_ptrs, record_num);
		Records records(schema_ptr, record_ptrs, record_num);
		return records;
	}

	size_t total_record_num = kFetchSize;
	Records records(schema_ptr, total_record_num);
	Record tmp_record(schema_ptr);
	TableIterator* iterator = getIterator();
	while (iterator->hasNext()) {
		tmp_record.transferData(iterator->next());
		if (!std::memcmp(param.value, tmp_record.getColumn(param.column_id), tmp_record.getColumnSize(param.column_id))) {
			records.insertRecord(tmp_record);
		}
	}
	delete iterator;
	return records;
}

Record SchemaTable::selectRecord(const ColumnPredicates& params) const {
	std::string pred_col_symbol = schema_ptr->getPredicateColumnSymbol(params);
	if (pred_col_symbol == primary_index_ptr.first) {
		std::string key_symbol = schema_ptr->getPredicateDataSymbol(params);
		char* address = primary_index_ptr.second->searchRecord(key_symbol);
		if (address != NULL) {
			Record record(schema_ptr);
			record.transferData(address);
			return record;
		} else return Record(NULL);
	}

	if (secondary_index_ptrs.find(pred_col_symbol) != secondary_index_ptrs.end()) {
		std::string key_symbol = schema_ptr->getPredicateDataSymbol(params);
		char* address = secondary_index_ptrs.at(pred_col_symbol)->searchRecord(key_symbol);
		if (address != NULL) {
			Record record(schema_ptr);
			record.transferData(address);
			return record;
		} else return Record(NULL);
	}

	Record record(schema_ptr);
	TableIterator* iterator = getIterator();
	while (iterator->hasNext()) {
		record.transferData(iterator->next());
		bool is_match = true;
		for (size_t i = 0; i < params.column_num; ++i) {
			if (std::memcmp(params.column_values[i], record.getColumn(params.column_ids[i]), record.getColumnSize(params.column_ids[i]))) {
				is_match = false;
				break;
			}
		}

		if (is_match) {
			delete iterator;
			return record;
		}
	}
	delete iterator;
	return Record(NULL);
}

Records SchemaTable::selectRecords(const ColumnPredicates& params) const {
	std::string sec_col_symbol = schema_ptr->getPredicateColumnSymbol(params);

	if (secondary_index_ptrs.find(sec_col_symbol) != secondary_index_ptrs.end()) {
		std::string ret_key = schema_ptr->getPredicateDataSymbol(params);
		char** record_ptrs = NULL;
		size_t record_num = 0;
		secondary_index_ptrs.at(sec_col_symbol)->searchRecords(ret_key, record_ptrs, record_num);
		Records records(schema_ptr, record_ptrs, record_num);
		return records;
	}

	size_t total_record_num = kFetchSize;
	Records records(schema_ptr, total_record_num);
	Record tmp_record(schema_ptr);
	TableIterator* iterator = getIterator();
	while (iterator->hasNext()) {
		tmp_record.transferData(iterator->next());
		bool is_match = true;
		for (size_t i = 0; i < params.column_num; ++i) {
			if (std::memcmp(params.column_values[i], tmp_record.getColumn(params.column_ids[i]), tmp_record.getColumnSize(params.column_ids[i]))) {
				is_match = false;
				break;
			}
		}

		if (is_match) {
			records.insertRecord(tmp_record);
		}
	}
	delete iterator;
	return records;
}


