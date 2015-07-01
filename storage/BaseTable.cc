#include "BaseTable.h"

using namespace IMDB::StorageEngine;

BaseTable::BaseTable(const size_t &record_size)
	: record_size(record_size), record_full_size(record_size + kRecordHeaderSize),
	record_num_per_block(kBlockSize/record_full_size), occupied_record_count(0),
	allocated_record_count(0), next_freespace_ptr(NULL)
{}

BaseTable::~BaseTable() {
	clear();
}

void BaseTable::clear() {
	for (auto block : blocks)
		block->release();
	
	blocks.clear();
	//free_list = std::queue<char*>();
	free_list.clear();
	next_freespace_ptr = NULL;
	occupied_record_count = allocated_record_count = 0;
}

TableIterator* BaseTable::getIterator() const {
	return new TableIterator(blocks[0], blocks[0]->data, record_full_size);
}
/*
TableIterator BaseTable::begin() {
	return TableIterator(blocks[0], blocks[0]->data, record_full_size);
}

TableIterator BaseTable::end() {
	return TableIterator(blocks[blocks.size() - 1], next_freespace_ptr, record_full_size);
}
*/
char* BaseTable::insertRecord(const char* record_ptr) {
	char* ans = NULL;
	{
		std::lock_guard<std::mutex> lock(table_mutex);
		if (allocated_record_count == occupied_record_count) {
			allocateNextBlock();
			ans = next_freespace_ptr;
			next_freespace_ptr += record_full_size;
		} else if (free_list.try_pop(ans)) {
			//char* append_ptr = free_list.front();
			//free_list.pop();
			//ans = append_ptr;
		} else {
			ans = next_freespace_ptr;
			next_freespace_ptr += record_full_size;
		}
		occupied_record_count++;
	}
	ans[0] = RecordFlag::OCCUPIED;
	memcpy(ans + kRecordHeaderSize, record_ptr, record_size);

	return ans + kRecordHeaderSize;
}

void BaseTable::deleteRecord(char* record_pos) {
	char* del_pos = record_pos - kRecordHeaderSize;
	free_list.push(del_pos);
	memset(del_pos, 0, kRecordHeaderSize);
	occupied_record_count--;
}

void BaseTable::allocateNextBlock() {
	Block* append_block = new Block(kBlockSize / record_full_size * record_full_size);
	if (blocks.size() > 0) blocks[blocks.size() - 1]->next_block = append_block;
	next_freespace_ptr = append_block->data;
	blocks.push_back(append_block);
	allocated_record_count += record_num_per_block;
}
