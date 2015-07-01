#include "ScrambledZipfianGenerator.h"

using namespace IMDB::Benchmark;

const double ScrambledZipfianGenerator::ZETAN = 26.46902820178302;
const double ScrambledZipfianGenerator::USED_ZIPFIAN_CONSTANT = 0.99;
const long ScrambledZipfianGenerator::ITEM_COUNT = 10000000000;

ScrambledZipfianGenerator::ScrambledZipfianGenerator(long min, long max, double zipfian_constant) {
	this->min = min;
	this->max = max;
	this->item_count = max - min + 1;
	if (zipfian_constant == USED_ZIPFIAN_CONSTANT) {
		gen = new ZipfianGenerator(0, ITEM_COUNT, zipfian_constant, ZETAN);
	} else gen = new ZipfianGenerator(0, ITEM_COUNT, zipfian_constant);
}

int ScrambledZipfianGenerator::nextLong() {
	long ret = gen->nextLong();
	ret = min + Utils::FNVhash64(ret) % item_count;
	setLastInt((int)ret);
	return ret;
}

double ScrambledZipfianGenerator::mean() {
	return ((double)(min + max)) / 2.0;
}