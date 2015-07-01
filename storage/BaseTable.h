#ifndef __BASE_TABLE_H__
#define __BASE_TABLE_H__

#include <vector>
#include <queue>
#include <cstring>
#include <mutex>

#include "MetaTypes.h"
#include "Block.h"
#include "TableIterator.h"
#include "tbb/concurrent_queue.h"

namespace IMDB {
	namespace StorageEngine {
		class BaseTable {
		public:
			BaseTable(const size_t &record_size);
			virtual ~BaseTable();

			const size_t& getOccupiedRecordCount() const {
				return occupied_record_count;
			}

			const size_t& getAllocatedRecordCount() const {
				return allocated_record_count;
			}

			const size_t& getTableSize() const {
				return occupied_record_count;
			}
		protected:
			TableIterator* getIterator() const;
			void clear();
			char* insertRecord(const char* record_ptr);
			void deleteRecord(char* record_pos);
		private:
			void allocateNextBlock();
			
			BaseTable(const BaseTable&);
			BaseTable & operator=(const BaseTable&);

			std::vector<Block*> blocks;
			const size_t record_size;
			const size_t record_full_size;
			const size_t record_num_per_block;
			size_t occupied_record_count;
			size_t allocated_record_count;
			
			tbb::concurrent_queue<char*> free_list;
			char* next_freespace_ptr;
			std::mutex table_mutex;
		};
	}
}

#endif
