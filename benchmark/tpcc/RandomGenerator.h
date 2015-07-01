#ifndef __TPCC_RANDOM_GENERATOR_H__
#define __TPCC_RANDOM_GENERATOR_H__

#include <string>
#include <cstdlib>
#include <unordered_set>
#include "TpccConstants.h"
#include <chrono>
#include <cstring>

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			class RandomGenerator {
			public:
				static int generateNonuniformInteger(const int& a, const int& x, const int& y) {
					int c = 0;
					if (a == 255) c = cLast;
					else if (a == 1023) c = cId;
					else if (a == 8191) c = orderlineItemId;
					return (((generateInteger(0, a) | generateInteger(x, y)) + c ) % (y - x + 1)) + x;
				}

				static int generateInteger(const int& min, const int& max) {
					return rand() % (max - min + 1) + min;
				}

				static int generateIntegerExcluding(const int& min, const int& max, const int& excluding) {
					int num = generateInteger(min, max - 1);
					if (num > excluding) num++;
					return num;
				}

				static double generateFixedPoint(const int& decimal_places, const double& min, const double& max) {
					int multiplier = 1;
					for (int i = 0; i < decimal_places; ++i)
						multiplier *= 10;

					int int_min = (int)(min * multiplier + 0.5);
					int int_max = (int)(max * multiplier + 0.5);

					return (double)(generateInteger(int_min, int_max) / (double)multiplier);
				}

				static std::string generateCString(const int& min_length, const int& max_length) {
					return generateRandomString(min_length, max_length, 'a', 26);
				}

				static std::string generateNString(const int& min_length, const int& max_length) {
					return generateRandomString(min_length, max_length, '0', 10);
				}

				static std::string generateRandomString(const int& min_length, const int& max_length, const char& base, const int& num_characters) {
					int length = generateInteger(min_length, max_length);
					std::string str;
					str.resize(length);
					for (int i = 0; i < length; ++i) {
						str[i] = (char)(base + generateInteger(0, num_characters - 1));
					}
					return str;
				}

				static std::string generateLastName(const int& number) {
					int index1 = number / 100;
					int index2 = (number / 10) % 10;
					int index3 = number % 10;
					return syllables[index1] + syllables[index2] + syllables[index3];
				}

				static std::string generateRandomLastName(const int& max_cid) {
					int min_cid = 999;
					if (max_cid - 1 < min_cid)
						min_cid = max_cid - 1;
					return generateLastName(generateNonuniformInteger(255, 0, min_cid));
				}

				static void selectUniqueIds(const int& numUnique, const int& min, const int& max, std::unordered_set<int>& rows) {
					for (int i = 0; i < numUnique; ++i) {
						int num = -1;
						while (num == -1 || rows.find(num) != rows.end())
							num = generateInteger(min, max);
						rows.insert(num);
					}
				}

				static void generateAddress(char* street_1, char* street_2, char* city, char* state, char* zip) {
					std::string street1_str = RandomGenerator::generateCString(MIN_STREET, MAX_STREET);
					memcpy(street_1, street1_str.c_str(), street1_str.size());
					std::string street2_str = RandomGenerator::generateCString(MIN_STREET, MAX_STREET);
					memcpy(street_2, street2_str.c_str(), street2_str.size());
					std::string city_str = RandomGenerator::generateCString(MIN_CITY, MAX_CITY);
					memcpy(city, city_str.c_str(), city_str.size());
					std::string state_str = RandomGenerator::generateCString(STATE, STATE);
					memcpy(state, state_str.c_str(), state_str.size());
					int length = ZIP_LENGTH - ZIP_SUFFIX.size();
					std::string zip_str = RandomGenerator::generateNString(length, length) + ZIP_SUFFIX;
					memcpy(zip, zip_str.c_str(), zip_str.size());
				}

				static std::string FillOriginal(std::string data) {
					int original_length = ORIGINAL_STRING.length();
					int position = RandomGenerator::generateInteger(0, data.length() - original_length);
					return data.substr(0, position) + ORIGINAL_STRING + data.substr(position + original_length);
				}

				static double generateTax() {
					return RandomGenerator::generateFixedPoint(TAX_DECIMALS, MIN_TAX, MAX_TAX);
				}

				static int64_t generateCurrentTime() {
					return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				}

				static int generateWarehouseId(const int& starting_warehouse, const int& ending_warehouse) {
					return RandomGenerator::generateInteger(starting_warehouse, ending_warehouse);
				}

				static int generateDistrictId(const int& num_districts_per_warehouse) {
					return RandomGenerator::generateInteger(1, num_districts_per_warehouse);
				}

				static int generateCustomerId(const int& num_customers_per_district) {
					return RandomGenerator::generateNonuniformInteger(1023, 1, num_customers_per_district);
				}

				static int generateItemId(const int& num_items) {
					return RandomGenerator::generateNonuniformInteger(8191, 1, num_items);
				}
			private:
				const static std::string syllables[10];
				const static int cLast;
				const static int cId;
				const static int orderlineItemId;
			};
		}
	}
}

#endif
