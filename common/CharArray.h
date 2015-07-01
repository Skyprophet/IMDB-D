#ifndef __CHAR_ARRAY_H__
#define __CHAR_ARRAY_H__

#include <cstring>
#include <cstdint>
namespace IMDB {
	struct CharArray {
		CharArray() : size(0), data(NULL) {}
		~CharArray() {
			release();
		}
	
		void hardCopy(const CharArray& other) {
			size = other.size;
			data = new char[size];
			memcpy(data, other.data, size);
		}

		void softCopy(const CharArray& other) {
			size = other.size;
			data = other.data;
		}

		void Memcpy(const uint32_t& offset, const CharArray& other) {
			memcpy(data + offset, other.data, other.size);
		}

		void Memcpy(const uint32_t& offset, const char* other, const int& size) {
			memcpy(data + offset, other, size);
		}

		void Memset(const uint32_t& offset, const int& value, const int& size) {
			memset(data + offset, value, size);
		}

		void allocate(const uint32_t& size) {
			this->size = size;
			this->data = new char[size];
			memset(data, 0, size);
		}

		void release() {
			size = 0;
			if (data != NULL) {
				delete[] data;
				data = NULL;
			}
		}

		uint32_t size;
		char* data;
	};
}

#endif
