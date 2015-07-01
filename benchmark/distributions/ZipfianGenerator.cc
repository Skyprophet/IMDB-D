#include "ZipfianGenerator.h"

using namespace IMDB::Benchmark;

const double ZipfianGenerator::ZIPFIAN_CONSTANT = 0.99;


ZipfianGenerator::ZipfianGenerator(long min, long max, double zipfian_constant, double zetan) {
	srand(time(NULL));

	items = max - min + 1;
	base = min;
	this->zipfian_constant = zipfian_constant;
	
	theta = zipfian_constant;
	zeta2theta = zeta(2, theta);
	
	alpha = 1.0/(1.0 - theta);
	this->zetan = zetan;
	count_for_zeta = items;
	eta = (1 - pow(2.0 / items, 1 - theta)) / (1 - zeta2theta / zetan);

	nextInt();
}

long ZipfianGenerator::nextLong(long item_count) {
	if (item_count != count_for_zeta) {
		std::lock_guard<std::mutex> lock(recompute_mutex);
		if (item_count > count_for_zeta) {
			zetan = zeta(count_for_zeta, item_count, theta, zetan);
			eta = (1 - pow(2.0 / items, 1 - theta)) / (1 - zeta2theta / zetan);
		} else if ((item_count < count_for_zeta) && (allow_item_count_decrease)) {
			//Note: have to start over zetan, for large item sets, this is very slow. SO DON'T DO IT!
			printf("WARNING: Recomputing Zipfian distribution. This is slow and should be avoided!! (item_count = %ld, count_for_zeta = %ld)\n", item_count, count_for_zeta);

			zetan = zeta(item_count, theta);
			eta = (1 - pow(2.0 / items, 1 - theta)) / (1 - zeta2theta / zetan);
		}
	}

	double u = RandomGenerator::generateDouble(0.0, 1.0);
	double uz = u * zetan;

	if (uz < 1.0) return 0;
	if (uz < 1.0 + pow (0.5, theta)) return 1;

	long ret = base + (long)((item_count) * pow(eta * u - eta + 1, alpha));
	setLastInt((int)ret);
	return ret;
}

double ZipfianGenerator::zetastatic(long st, long n, double theta, double initialsum) {
	double sum = initialsum;
	for (long i = st; i < n; ++i)
		sum += 1 / (pow(i + 1, theta));
	return sum;
}

double ZipfianGenerator::zetastatic(long n, double theta) {
	return zetastatic(0, n, theta, 0);
}