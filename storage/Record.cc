#include "Record.h"

using namespace IMDB::StorageEngine;
using namespace IMDB;

void Record::transferData(char* data_ptr) {
	this->data_ptr = data_ptr;
}

char* Record::fetchData() const {
	return data_ptr;
}

const Schema* Record::getSchema() const {
	return schema_ptr;
}

void Record::setColumn(const size_t& column_id, const char* data) {
	assert(schema_ptr->getColumnType(column_id) != ValueType::VARCHAR);
	memcpy(data_ptr + schema_ptr->getColumnOffset(column_id), data, getColumnSize(column_id));
}

void Record::setColumn(const size_t& column_id, const CharArray& data) {
	assert(schema_ptr->getColumnType(column_id) == ValueType::VARCHAR && schema_ptr->getColumnSize(column_id) >= data.size);
	memcpy(data_ptr + schema_ptr->getColumnOffset(column_id), data.data, data.size);
}

void Record::setColumn(const size_t& column_id, const char* data_str, const size_t& data_size) {
	assert(schema_ptr->getColumnType(column_id) == ValueType::VARCHAR && schema_ptr->getColumnSize(column_id) >= data_size);
	memcpy(data_ptr + schema_ptr->getColumnOffset(column_id), data_str, data_size);
}

void Record::setColumn(const size_t& column_id, const std::string& data) {
	assert(schema_ptr->getColumnType(column_id) == ValueType::VARCHAR && schema_ptr->getColumnSize(column_id) >= data.size());
	memcpy(data_ptr + schema_ptr->getColumnOffset(column_id), data.c_str(), data.size());
}

char* Record::getColumn(const size_t& column_id) const {
	return data_ptr + schema_ptr->getColumnOffset(column_id);
}

char* Record::getColumn(const size_t& column_id, char* data) const {
	assert(schema_ptr->getColumnType(column_id) == ValueType::VARCHAR);
	memcpy(data, data_ptr + schema_ptr->getColumnOffset(column_id), getColumnSize(column_id));
	return data;
}

CharArray& Record::getColumn(const size_t& column_id, CharArray& data) const {
	assert(schema_ptr->getColumnType(column_id) == ValueType::VARCHAR);
	data.size = getColumnSize(column_id);
	data.data = new char[data.size];
	memcpy(data.data, data_ptr + schema_ptr->getColumnOffset(column_id), data.size);
	return data;
}

void Record::getColumn(const size_t& column_id, std::string& data) const {
	assert(schema_ptr->getColumnType(column_id) == ValueType::VARCHAR);
	data.assign(data_ptr + schema_ptr->getColumnOffset(column_id), 0, getColumnSize(column_id));
}

const size_t& Record::getColumnSize(const size_t& column_id) const {
	return schema_ptr->getColumnSize(column_id);
}

const size_t& Record::getRecordSize() const {
	return schema_ptr->getSchemaSize();
}

const size_t& Record::getColumnCount() const {
	return schema_ptr->getColumnCount();
}

std::string Record::getPrimaryKeySymbol() const {
	size_t cur_offset = 0;
	size_t key_length = schema_ptr->getPrimaryKeyLength();
	if (key_length == 0)
		return std::string(data_ptr, schema_ptr->getSchemaSize());
	char* key_str = new char[key_length];
	for (size_t i = 0; i < schema_ptr->getPrimaryColumnCount(); ++i) {
		memcpy(key_str + cur_offset, getColumn(schema_ptr->getPrimaryColumnId(i)), schema_ptr->getPrimaryColumnSize(i));
		cur_offset += schema_ptr->getPrimaryColumnSize(i);
	}
	std::string ret_key = std::string(key_str, key_length);
	delete[] key_str;
	return ret_key;
}

std::string Record::getKeySymbol(const size_t* index_cols, const size_t& index_col_num) const {
	size_t cur_offset = 0;
	size_t key_length = schema_ptr->getKeyLength(index_cols, index_col_num);
	char* key_str = new char[key_length];
	for (size_t i = 0; i < index_col_num; ++i) {
		memcpy(key_str + cur_offset, getColumn(index_cols[i]), schema_ptr->getColumnSize(index_cols[i]));
		cur_offset += schema_ptr->getColumnSize(index_cols[i]);
	}
	std::string ret_key = std::string(key_str, key_length);
	delete[] key_str;
	return ret_key;
}

const char* ConstRecord::fetchData() const {
	return data_ptr;
}

const char* ConstRecord::getColumn(const size_t& column_id) const {
	return data_ptr + schema_ptr->getColumnOffset(column_id);
}

char* ConstRecord::getColumn(const size_t& column_id, char* data) const {
	assert(schema_ptr->getColumnType(column_id) != ValueType::VARCHAR);
	memcpy(data, data_ptr + schema_ptr->getColumnOffset(column_id), schema_ptr->getColumnSize(column_id));
	return data;
}

CharArray& ConstRecord::getColumn(const size_t& column_id, CharArray& data) const {
	assert(schema_ptr->getColumnType(column_id) == ValueType::VARCHAR);
	data.size = schema_ptr->getColumnSize(column_id);
	data.data = new char[data.size];
	memcpy(data.data, data_ptr + schema_ptr->getColumnOffset(column_id), data.size);
	return data;
}

void ConstRecord::getColumn(const size_t& column_id, std::string& data) const {
	assert(schema_ptr->getColumnType(column_id) == ValueType::VARCHAR);
	data.assign(data_ptr + schema_ptr->getColumnOffset(column_id), 0, schema_ptr->getColumnSize(column_id));
}

const size_t& ConstRecord::getColumnSize(const size_t& column_id) const {
	return schema_ptr->getColumnSize(column_id);
}

const size_t& ConstRecord::getRecordSize() const {
	return schema_ptr->getSchemaSize();
}

const size_t& ConstRecord::getColumnCount() const {
	return schema_ptr->getColumnCount();
}

std::string ConstRecord::getPrimaryKeySymbol() const {
	size_t cur_offset = 0;
	size_t key_length = schema_ptr->getPrimaryKeyLength();
	if (key_length == 0) return std::string(data_ptr, schema_ptr->getSchemaSize());
	char* key_str = new char[key_length];
	for (size_t i = 0; i < schema_ptr->getPrimaryColumnCount(); ++i) {
		memcpy(key_str + cur_offset, getColumn(schema_ptr->getPrimaryColumnId(i)), schema_ptr->getPrimaryColumnSize(i));
		cur_offset += schema_ptr->getPrimaryColumnSize(i);
	}
	std::string ret_key = std::string(key_str, key_length);
	delete[] key_str;
	return ret_key;
}

std::string ConstRecord::getKeySymbol(const size_t* index_cols, const size_t& index_col_num) const {
	size_t cur_offset = 0;
	size_t key_length = schema_ptr->getKeyLength(index_cols, index_col_num);
	char* key_str = new char[key_length];
	for (size_t i = 0; i < index_col_num; ++i) {
		memcpy(key_str + cur_offset, getColumn(index_cols[i]), schema_ptr->getColumnSize(index_cols[i]));
		cur_offset += schema_ptr->getColumnSize(index_cols[i]);
	}
	std::string ret_key = std::string(key_str, key_length);
	delete[] key_str;
	return ret_key;
}
