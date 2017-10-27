# macro_robinhood_hash
Flexible C macros implementation of fixed size array-based Robin Hood hash table with backward-shift deletion.

Implemented by Vitaly "_Vi" Shukela in 2017. License = MIT or Apache 2.0.

Don't shrink this hash table by sequentially iterating it and inserting elements into a smaller table.

## Examples

* example.c - basic int-to-char table
* fuzzer.cpp - checks integrity of the table by issuing random operations and comparing with std::set
