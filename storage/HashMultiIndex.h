#ifndef __HASH_MULTI_INDEX_H__
#define __HASH_MULTI_INDEX_H__

#include "BaseMultiIndex.h"
#include <map>

namespace IMDB {
	namespace StorageEngine {
		typedef std::unordered_multimap<std::string, char*> hash_index_t;

		class HashMultiIndex : public BaseMultiIndex {
		public:
			HashMultiIndex() {}
			virtual ~HashMultiIndex() {}

			virtual void insertRecord(const std::string& key, char* address) {
				hash_index.insert(std::pair<std::string, char*>(key, address));
			}

			//Beware! This command will erase all elements with the key!!
			virtual void deleteRecord(const std::string& key) {
				hash_index.erase(key);
			}

			virtual char* searchRecord(const std::string& key) const {
				if (hash_index.find(key) == hash_index.end())
					return NULL;
				else return hash_index.find(key)->second;
			}

			virtual void searchRecords(const std::string& key, char**& records, size_t &num) const {
				num = hash_index.count(key);
				if (num == 0) records = NULL;
				else  {
					records = new char*[num];
					auto iterator = hash_index.equal_range(key).first;
					for (size_t i = 0; i < num; ++i) {
						records[i] = iterator->second;
						++iterator;
					}
				}
			}

			virtual size_t getSize() const {
				return hash_index.size();
			}
		private:
			HashMultiIndex(const HashMultiIndex&);
			HashMultiIndex& operator=(const HashMultiIndex&);

			hash_index_t hash_index;
		};
	}
}

#endif