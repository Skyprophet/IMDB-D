#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "BaseTable.h"
#include <cstdint>

namespace IMDB {
	namespace StorageEngine {
		class Block {
		public:
			Block(uint32_t size) {
				data = new char[size];
				memset(data, 0, size);
				end_ptr = data + size;
				next_block = NULL;
			}

			~Block() {
				release();
			}

			void release() {
				if (data != NULL) {
					delete[] data;
					data = NULL;
				}
			}
		private:
			friend class BaseTable;
			friend class TableIterator;
			
			char* data;
			char* end_ptr;
			Block* next_block;
		};
	}
}

#endif
