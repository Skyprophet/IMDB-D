#include "Schema.h"
#include <cstring>

using namespace IMDB::StorageEngine;

void Schema::insertColumn(const ColumnInfo& column) {
	columns.emplace_back(column);
	columns.at(columns.size() - 1).column_offset = column_offset;
	if (column.is_primary == true) {
		assert(primary_col_num < 5);
		primary_key_length += column.column_size;
		primary_cols[primary_col_num] = column_count;
		primary_col_sizes[primary_col_num] = column.column_size;
		primary_col_num++;
	}
	column_count++;
	column_offset += column.column_size;
}

const ColumnInfo& Schema::getColumn(const size_t &column_id) const {
	assert(column_id < column_count);
	return columns.at(column_id);
}

const size_t& Schema::getSchemaSize() const {
	return column_offset;
}

const size_t& Schema::getColumnCount() const {
	return column_count;
}

const std::string& Schema::getColumnName(const size_t& index) const {
	return getColumn(index).column_name;
}

const ValueType& Schema::getColumnType(const size_t& index) const {
	return getColumn(index).column_type;
}

const size_t& Schema::getColumnOffset(const size_t& index) const {
	return getColumn(index).column_offset;
}

const size_t& Schema::getColumnSize(const size_t& index) const {
	return getColumn(index).column_size;
}

bool Schema::isColumnPrimary(const size_t& index) const {
	return getColumn(index).is_primary;
}

const size_t& Schema::getPrimaryColumnCount() const {
	return primary_col_num;
}

const size_t& Schema::getPrimaryColumnId(const size_t& col_index) const {
	assert(col_index < primary_col_num);
	return primary_cols[col_index];
}

const size_t& Schema::getPrimaryColumnSize(const size_t& col_index) const {
	assert(col_index < primary_col_num);
	return primary_col_sizes[col_index];
}

const size_t& Schema::getPrimaryKeyLength() const {
	return primary_key_length;
}

size_t Schema::getKeyLength(const size_t* index_cols, const size_t& index_col_num) const {
	size_t sum = 0;
	for (size_t i = 0; i < index_col_num; ++i)
		sum += columns.at(index_cols[i]).column_size;
	return sum;
}

std::string Schema::getPrimaryColumnSymbol() const {
	return std::string(reinterpret_cast<const char*>(primary_cols), sizeof(size_t) * primary_col_num);
}

std::string Schema::getPredicateColumnSymbol(const ColumnPredicate& param) const {
	return std::string(reinterpret_cast<const char*>(param.column_id), sizeof(size_t));
}

std::string Schema::getPredicateDataSymbol(const ColumnPredicate& param) const {
	return std::string(param.value, getColumnSize(param.column_id));
}

std::string Schema::getPredicateColumnSymbol(const ColumnPredicates& params) const {
	return std::string(reinterpret_cast<const char*>(params.column_ids), sizeof(size_t)*params.column_num);
}

std::string Schema::getPredicateDataSymbol(const ColumnPredicates& params) const {
	size_t key_length = getKeyLength(params.column_ids, params.column_num);
	size_t cur_offset = 0;
	char* key_str = new char[key_length];
	for (size_t i = 0; i < params.column_num; ++i) {
		memcpy(key_str + cur_offset, params.column_values[i], getColumnSize(params.column_ids[i]));
		cur_offset += getColumnSize(params.column_ids[i]);
	}
	std::string ret_symbol = std::string(key_str, key_length);
	delete[] key_str;
	return ret_symbol;
}
