#ifndef __SCHEMA_H__
#define __SCHEMA_H__

#include <vector>
#include <cassert>
#include "MetaTypes.h"

namespace IMDB {
	namespace StorageEngine {
		class Schema {
		public:
			Schema() : column_count(0), column_offset(0), primary_key_length(0), primary_col_num(0) {}
			~Schema() {}

			void insertColumn(const ColumnInfo &column);
			
			const ColumnInfo& getColumn(const size_t& column_id) const;
			const size_t& getSchemaSize() const;
			const size_t& getColumnCount() const;

			const std::string& getColumnName(const size_t& index) const;
			const ValueType& getColumnType(const size_t& index) const;
			const size_t& getColumnOffset(const size_t& index) const;
			const size_t& getColumnSize(const size_t& index) const;
			bool isColumnPrimary(const size_t& index) const;

			const size_t& getPrimaryColumnCount() const;
			const size_t& getPrimaryColumnId(const size_t& col_index) const;
			const size_t& getPrimaryColumnSize(const size_t& col_index) const;
			const size_t& getPrimaryKeyLength() const;

			size_t getKeyLength(const size_t* index_cols, const size_t& index_col_num) const;
			std::string getPrimaryColumnSymbol() const;
			std::string getPredicateColumnSymbol(const ColumnPredicate& param) const;
			std::string getPredicateDataSymbol(const ColumnPredicate& param) const;
			std::string getPredicateColumnSymbol(const ColumnPredicates& params) const;
			std::string getPredicateDataSymbol(const ColumnPredicates& param) const;
		private:
			Schema(const Schema&);
			Schema& operator=(const Schema&);

			std::vector<ColumnInfo> columns;
			size_t column_count;
			size_t column_offset;
			size_t primary_key_length;
			size_t primary_col_num;
			size_t primary_cols[5];
			size_t primary_col_sizes[5];
		};
	}
}

#endif
