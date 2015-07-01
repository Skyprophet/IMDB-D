#include "Util.h"

using namespace IMDB::Benchmark;

const int Utils::FNV_offset_basis_32 = 0x811c9dc5;
const int Utils::FNV_prime_32 = 16777619;
const long Utils::FNV_offset_basis_64 = 0xcbf29ce484222325;
const long Utils::FNV_prime_64 = 1099511628211;

long Utils::hash(long val) {
	return FNVhash64(val);
}

int Utils::FNVhash32(int val) {
	int hashval = FNV_offset_basis_32;

	for (int i = 0; i < 4; ++i) {
		int octet = val & 0x00ff;
		val = val >> 8;

		hashval = hashval ^ octet;
		hashval = hashval * FNV_prime_32;
	}

	return abs(hashval);
}

long Utils::FNVhash64(long val) {
	long hashval = FNV_offset_basis_64;

	for (int i = 0; i < 8; ++i) {
		long octet = val & 0x00ff;
		val = val >> 8;

		hashval = hashval ^ octet;
		hashval = hashval * FNV_prime_64;
	}

	return abs(hashval);
}