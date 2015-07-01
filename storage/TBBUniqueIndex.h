#ifndef __TBB_UNIQUE_INDEX_H__
#define __TBB_UNIQUE_INDEX_H__

#include "tbb/concurrent_hash_map.h"
#include "BaseUniqueIndex.h"

namespace IMDB {
	namespace StorageEngine {
		class TBBUniqueIndex : public BaseUniqueIndex {
		private:
			typedef tbb::concurrent_hash_map<std::string, char*> TBBHashMap;
			typedef TBBHashMap::iterator iterator;
			typedef TBBHashMap::const_iterator const_iterator;
			typedef TBBHashMap::const_accessor const_accessor;
		public:
			TBBUniqueIndex() {}
			virtual ~TBBUniqueIndex() {}

			virtual void insertRecord(const std::string& key, char* ptr) {
				map.insert(std::make_pair(key, ptr));
			}

			virtual void deleteRecord(const std::string& key) {
				map.erase(key);
			}

			virtual char* searchRecord(const std::string& key) const {
				const_accessor a;
				if (map.find(a, key)) return a->second;
				return NULL;
			}

			virtual size_t getSize() const {
				return map.size();
			}
		private:
			TBBUniqueIndex(const TBBUniqueIndex&);
			TBBUniqueIndex& operator=(const TBBUniqueIndex&);

			TBBHashMap map;
		};
	}
}

#endif
