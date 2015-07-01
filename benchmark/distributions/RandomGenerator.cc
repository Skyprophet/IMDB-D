#include "RandomGenerator.h"

using namespace IMDB::Benchmark;

namespace IMDB {
	namespace Benchmark {
		std::random_device rd;
		std::mt19937 gen(rd());
	}
}

int RandomGenerator::generateInteger(const int& min, const int& max) {
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

int RandomGenerator::generateDouble(const double& min, const double& max) {
	std::uniform_real_distribution<double> dis(min, max);
	return dis(gen);
}

long RandomGenerator::generateLong(const long& min, const long& max) {
	std::uniform_int_distribution<long> dis(min, max);
	return dis(gen);
}

std::string RandomGenerator::generateCString(const int& min_length, const int& max_length) {
	return generateRandomString(min_length, max_length, 'a', 26);
}

std::string RandomGenerator::generateNString(const int& min_length, const int& max_length) {
	return generateRandomString(min_length, max_length, '0', 10);
}

std::string RandomGenerator::generateRandomString(const int& min_length, const int& max_length, const char& base, const int& num_characters) {
	int length = generateInteger(min_length, max_length);
	std::string str;
	str.resize(length);
	for (int i = 0; i < length; ++i) {
		str[i] = (char)(base + generateInteger(0, num_characters - 1));
	}
	return str;
}
