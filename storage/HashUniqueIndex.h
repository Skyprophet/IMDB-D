#ifndef __HASH_UNIQUE_INDEX_H__
#define __HASH_UNIQUE_INDEX_H__

#include "BaseUniqueIndex.h"
#include <unordered_map>

namespace IMDB {
	namespace StorageEngine {
		class HashUniqueIndex : public BaseUniqueIndex {
		public:
			HashUniqueIndex() {}
			virtual ~HashUniqueIndex() {}

			virtual void insertRecord(const std::string& key, char* address) {
				hash_index.insert(std::pair<std::string, char*>(key, address));
			}

			virtual void deleteRecord(const std::string& key) {
				hash_index.erase(key);
			}

			virtual char* searchRecord(const std::string& key) const {
				if (hash_index.find(key) == hash_index.end())
					return NULL;
				else return hash_index.at(key);
			}

			virtual size_t getSize() const {
				return hash_index.size();
			}
		private:
			HashUniqueIndex(const HashUniqueIndex&);
			HashUniqueIndex& operator=(const HashUniqueIndex&);

			std::unordered_map<std::string, char*> hash_index;
		};
	}
}

#endif