#ifndef __SCHEMA_TABLE_ITERATOR_H__
#define __SCHEMA_TABLE_ITERATOR_H__

#include "TableIterator.h"

namespace IMDB {
	namespace StorageEngine {
		class SchemaTableIterator {
		public:
			SchemaTableIterator(const Schema* schema_ptr, TableIterator* iterator)
				: schema_ptr(schema_ptr), iterator(iterator) {}
			~SchemaTableIterator() {
				delete iterator;
				iterator = NULL;
			}

			bool hasNext() {
				return iterator->hasNext();
			}

			Record next() {
				Record record(schema_ptr);
				record.transferData(iterator->next());
				return record;
			}
		private:
			const Schema* const schema_ptr;
			TableIterator* iterator;
		};
	}
}

#endif
