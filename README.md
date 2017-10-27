# macro_robinhood_hash
Flexible C macros implementation of fixed size array-based Robin Hood hash table with backward-shift deletion.

Implemented by Vitaly "_Vi" Shukela in 2017. License = MIT or Apache 2.0.

Don't shrink this hash table by sequentially iterating it and inserting elements into a smaller table.

## Examples

* example.c - basic int-to-char table
* example_stringkeys.c - chararray-keyed table
* fuzzer.cpp - checks integrity of the table by issuing random operations and comparing with std::set

## How to use

* Design the table, so each cell can be easily checked for emptiness, keys stored inside cells somehow and preferred bucket number (i.e. hash) is easily gettable from a key (or even stored inside). One value should be designated to mean empty cell. Zeroeth bucket is also reserved for newly incoming entries and is never preferred.
* Implement elementrary functions for getting keys and/or values, checking for emptiness, swapping two cell contents and so on using C macros prefixed with some your prefix
* Use ROBINHOOD_HASH_* macros, specifying that prefix. "u" is just argument for all your macros.
