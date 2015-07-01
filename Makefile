CC = g++
CXXFLAGS = -std=c++0x -Wall -O2
LINKERS = -pthread -ltbb

STORAGE_SRCFILES = storage/Record.cc \
	storage/Schema.cc \
	storage/SchemaTable.cc \
	storage/BaseTable.cc
DISTRIBUTIONS_SRCFILES = benchmark/distributions/RandomGenerator.cc \
	benchmark/distributions/ZipfianGenerator.cc \
	benchmark/distributions/Util.cc \
	benchmark/distributions/ScrambledZipfianGenerator.cc
TPCC_SRCFILES = benchmark/tpcc/TpccSource.cc \
	benchmark/tpcc/TpccSerialProcessor.cc \
	benchmark/tpcc/TpccSchema.cc \
	benchmark/tpcc/TpccRecords.cc \
	benchmark/tpcc/TpccPopulator.cc \
	benchmark/tpcc/TpccMain.cc \
	benchmark/tpcc/TpccBaseProcessor.cc \
	benchmark/tpcc/RandomGenerator.cc \
	benchmark/tpcc/TpccBatchProcessor.cc \
	benchmark/tpcc/TpccTPBatchProcessor.cc
TATP_SRCFILES = benchmark/tatp/TatpBaseProcessor.cc \
	benchmark/tatp/TatpSchema.cc \
	benchmark/tatp/TatpSerialProcessor.cc \
	benchmark/tatp/TatpRecords.cc \
	benchmark/tatp/TatpPopulator.cc \
	benchmark/tatp/TatpMain.cc \
	benchmark/tatp/TatpBatchProcessor.cc \
	benchmark/tatp/TatpTPBatchProcessor.cc

STORAGE_OBJFILES = $(patsubst storage/%.cc, obj/storage/%.o, $(STORAGE_SRCFILES))
DISTRIBUTIONS_OBJFILES = $(patsubst benchmark/distributions/%.cc, obj/benchmark/distributions/%.o, $(DISTRIBUTIONS_SRCFILES))
TPCC_OBJFILES = $(patsubst benchmark/tpcc/%.cc, obj/benchmark/tpcc/%.o, $(TPCC_SRCFILES))
TATP_OBJFILES = $(patsubst benchmark/tatp/%.cc, obj/benchmark/tatp/%.o, $(TATP_SRCFILES))

ESSENTIAL_OBJFILES = $(STORAGE_OBJFILES) 
BENCHMARK_ESSENTIAL_OBJFILES = $(ESSENTIAL_OBJFILES) $(DISTRIBUTIONS_OBJFILES)

EXECUTABLE = $(BENCHMARKS)

BENCHMARKS = bin/tpcc_main \
	bin/tatp_main

all: $(EXECUTABLE)

benchmark: $(BENCHMARKS)

storage: $(ESSENTIAL_OBJFILES)

bin/tpcc_main: $(BENCHMARK_ESSENTIAL_OBJFILES) $(TPCC_OBJFILES)
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LINKERS)

bin/tatp_main: $(BENCHMARK_ESSENTIAL_OBJFILES) $(TATP_OBJFILES)
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LINKERS)

obj/storage/%.o: storage/%.cc
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -c $< -o $@

obj/benchmark/distributions/%.o: benchmark/distributions/%.cc
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -c $< -o $@

obj/benchmark/tpcc/%.o: benchmark/tpcc/%.cc
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -c $< -o $@

obj/benchmark/tatp/%.o : benchmark/tatp/%.cc
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf obj
	rm -rf bin
