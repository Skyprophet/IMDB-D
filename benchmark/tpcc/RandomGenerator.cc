#include "RandomGenerator.h"

using namespace IMDB::Benchmark::TPCC;

const std::string RandomGenerator::syllables[10] = {
	"BAR", "OUGHT", "ABLE", "PRI", "PRES", "ESE", "ANTI", "CALLY", "ATION", "EING"
};

const int RandomGenerator::cLast = RandomGenerator::generateInteger(0, 255);
const int RandomGenerator::cId = RandomGenerator::generateInteger(0, 1023);
const int RandomGenerator::orderlineItemId = RandomGenerator::generateInteger(0, 8191);
