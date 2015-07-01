#ifndef __TABLE_ITERATOR_H__
#define __TABLE_ITERATOR_H__

#include "Block.h"

namespace IMDB {
	namespace StorageEngine {
		class TableIterator {
		public:
			TableIterator(Block *block, char *ptr, const uint32_t& record_size)
				: cur_block(block), m_ptr(ptr), record_size(record_size) {
			}
			~TableIterator() {}

			bool hasNext() {
				while (cur_block->next_block != NULL || m_ptr != cur_block->end_ptr) {
					if (m_ptr == cur_block->end_ptr) {
						cur_block = cur_block->next_block;
						m_ptr = cur_block->data;
					}

					if (m_ptr[0] == RecordFlag::OCCUPIED) return true;

					m_ptr += record_size;

				}
				return false;
			}

			char* next() {
				while (true) {
					if (m_ptr == cur_block->end_ptr) {
						cur_block = cur_block->next_block;
						m_ptr = cur_block->data;
					}
					if (m_ptr[0] == RecordFlag::OCCUPIED) {
						char* ans = m_ptr + kRecordHeaderSize;
						m_ptr += record_size;
						return ans;
					} else {
						m_ptr += record_size;
					}
				}
			}
		private:
			Block* cur_block;
			char* m_ptr;
			const uint32_t record_size;
		};
	}
}

#endif
