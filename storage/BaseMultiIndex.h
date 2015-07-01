#ifndef __BASE_MULTI_INDEX_H__
#define __BASE_MULTI_INDEX_H__

#include <string>

namespace IMDB {
	namespace StorageEngine {
		class BaseMultiIndex {
		public:
			BaseMultiIndex() {}
			virtual ~BaseMultiIndex() {}

			virtual void insertRecord(const std::string&, char*) = 0;
			virtual void deleteRecord(const std::string&) = 0;
			virtual char* searchRecord(const std::string&) const = 0;
			virtual void searchRecords(const std::string&, char**&, size_t&) const = 0;
			virtual size_t getSize() const = 0;
		private:
			BaseMultiIndex(const BaseMultiIndex&);
			BaseMultiIndex& operator=(const BaseMultiIndex&);
		};
	}
}

#endif
