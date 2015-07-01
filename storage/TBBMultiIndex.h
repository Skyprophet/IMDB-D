#ifndef __TBB_MULTI_INDEX_H__
#define __TBB_MULTI_INDEX_H__

#include "BaseMultiIndex.h"
#include "tbb/tbb_allocator.h"
#include "tbb/concurrent_unordered_map.h"

namespace IMDB {
	namespace StorageEngine {
		class TBBMultiIndex : public BaseMultiIndex {
		private:
			typedef tbb::concurrent_unordered_multimap<std::string, char*> TBBMultiMap;
			typedef TBBMultiMap::const_iterator const_iterator;
		public:
			TBBMultiIndex() {}
			virtual ~TBBMultiIndex() {}

			virtual void insertRecord(const std::string& key, char* ptr) {
				map.insert(std::make_pair(key, ptr));
			}

			//TODO this erase is not thread safe.
			virtual void deleteRecord(const std::string& key) {
				map.unsafe_erase(key);
			}

			virtual char* searchRecord(const std::string& key) const {
				const_iterator it = map.find(key);
				if (it == map.end()) return NULL;
				else return it->second;
			}

			virtual void searchRecords(const std::string& key, char**& records, size_t& num) const {
				num = map.count(key);
				if (num == 0) records = NULL;
				else {
					records = new char*[num];
					const_iterator it = map.equal_range(key).first;
					for (size_t i = 0; i < num; ++i) {
						records[i] = it->second;
						++it;
					}
				}
			}
			
			virtual size_t getSize() const {
				return map.size();
			}
		private:
			TBBMultiIndex(const TBBMultiIndex&);
			TBBMultiIndex& operator=(const TBBMultiIndex&);

			TBBMultiMap map;
		};
	}
}

#endif
