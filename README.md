# README #
IMDB-D is the project for my SJTU Bachelor Thesis "Deterministic Main-Memory Database System". This system aims at achieving Strong Execution Determinism (SED) while fully utilizing multi-core power in a transational DBMS.

Strong Execution Determinsim (SED) requires that the database system schedules a transaction queue with serializability while the serialized order is always equivalent to the original queue order. This property is stronger than execution determinism proposed by Abadi et. al. in Calvin, which requires the serialized order is always equivalent to some specific order. SED is important for those applications that ask for strong execution order guarantee, such as log replaying. It can also help eliminating two-phase commit protocols from a distributed database system. To achieve this property, we introduce a concurrency control designing paradigm based on stored procedure analyzing. In our solution, dependencies within and between transactions are analyzed before execution. With the help of this, we can design a customed concurrency control scheme for a specific application.

In this system, we implemented a simple but efficient storage engine and apply our designing paradigm on two popular benchmarks (TPC-C & TATP). We conduct a set of experiments and found our solution scales well in multi-core environment.

## Dependency ##
This system requires following compiler/packages to be installed before building.

- gcc/g++ 4.8+
- boost 1.55+
- Intel Thread Building Blocks

# Build #
Simply `make` and enjoy it!!

