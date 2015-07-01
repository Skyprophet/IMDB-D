#ifndef __BASE_UNIQUE_INDEX_H__
#define __BASE_UNIQUE_INDEX_H__

#include <string>

namespace IMDB {
	namespace StorageEngine {
		class BaseUniqueIndex {
		public:
			BaseUniqueIndex() {}
			virtual ~BaseUniqueIndex() {}

			virtual void insertRecord(const std::string&, char*) = 0;
			virtual void deleteRecord(const std::string&) = 0;
			virtual char* searchRecord(const std::string&) const = 0;
			virtual size_t getSize() const = 0;
		private:
			BaseUniqueIndex(const BaseUniqueIndex&);
			BaseUniqueIndex& operator=(const BaseUniqueIndex&);
		};
	}
}


#endif
