#ifndef ROBINHOOD_HASH_H
#define ROBINHOOD_HASH_H

// Flexible C macros implementation of fixed size array-based Robin Hood hash table with backward-shift deletion.
// Implemented by Vitaly "_Vi" Shukela in 2017. License = MIT or Apache 2.0.

// Don't shrink this hash table by sequentially iterating it and inserting elements into a smaller table

// "X macro" pattern is expected to be for this parameters:
//  "x" part is variable, substitude it with your identifier and specify it later as "tbl"

// #define x_setvalue(u, index, key, val) is a macro to set value to the table cell
// #define x_setnil(u, index) is a macro that marks table entry as empty
// #define x_swap(u, index1, index2) is a macro that swaps two entries in the table
// #define x_nilvalue(u) is a value siganlizing that key is not found
// #define x_getvalue(u, index) is a macro to get value of the table cell
// #define x_getkey(u, index) is a macro for obtaining key for given index of the table.
// #define x_keysequal(u, key1,key2) is a macro that should return true when keys are equal
// #define x_isnil(u, index) is a macro that checks if table entry is empty
// #define x_n_elem(u) should point to number of buckets in hash table
// #define x_getbucket(u, key) is a macro for obtaining adviced 
//      index of the table of the given key. Should be in range [1, n_elem[.
// #define x_overflow(u) is a macro that is called when table is full
// #define x_removefailed(u,key) is a macro that called when trying to remove nonexisting element
// u is a "user data" argument that is specified in all user requests

// hash table entry should consist only of key and value and x_setvalue should completely set an entry
// If you don't need value just use reuse key as value

// API:
#define ROBINHOOD_HASH_SET(tbl, u, key, value) \
       _ROBINHOOD_HASH_SET(tbl, u, key, value)
       
#define ROBINHOOD_HASH_GET(tbl, u, key, assignme) \
       _ROBINHOOD_HASH_GET(tbl, u, key, assignme)
       
#define ROBINHOOD_HASH_DEL(tbl, u, key) \
       _ROBINHOOD_HASH_DEL(tbl, u, key)
       
#define ROBINHOOD_HASH_SIZE(tbl, u, assignme) \
       _ROBINHOOD_HASH_SIZE(tbl, u, assignme)
       
#define ROBINHOOD_HASH_CLEAR(tbl, u) \
       _ROBINHOOD_HASH_CLEAR(tbl, u)

// Impl:
       
#include <stddef.h> // size_t

#define _ROBINHOOD_HASH_TYPICAL_INIT(u, key, getbucket) \
    size_t _rh_i = getbucket(u, key); \
    size_t _rh_i_orig = _rh_i; \
    size_t _rh_temperature = 0;
    
#define _ROBINHOOD_HASH_TYPICAL_INCREMENT(breakcode, n_elem) \
    _rh_temperature += 1; \
    _rh_i += 1; \
    if (_rh_i>=n_elem) _rh_i=1; \
    if (_rh_i == _rh_i_orig) { \
        breakcode \
        break; \
    }

#define _ROBINHOOD_HASH_DEBUGPRINT(tbl, u) { \
    size_t _rh_i; \
    printf("RBHDP: "); \
    for(_rh_i=1; _rh_i < tbl##_n_elem(u); ++_rh_i) { \
        printf("%02d:",_rh_i); \
        if (tbl##_isnil(u, _rh_i)) { \
            printf("___+__ "); \
        } else { \
            printf("%03u+%02d ", tbl##_getkey(u,_rh_i), tbl##_getbucket(u,tbl##_getkey(u,i))); \
        } \
    } \
    printf("\n"); \
}

#define _ROBINHOOD_HASH_SET(tbl, u, key, value)  { \
    _ROBINHOOD_HASH_TYPICAL_INIT(u, key, tbl##_getbucket) \
    tbl##_setvalue(u, 0, key, value); \
    int _rh_check_for_match = 1; \
    for(;;) { \
        if (tbl##_isnil(u, _rh_i)) { \
            tbl##_setvalue(u, _rh_i, tbl##_getkey(u, 0), tbl##_getvalue(u,0)); \
            break; \
        } else { \
            if (_rh_check_for_match && (tbl##_keysequal(u, tbl##_getkey(u, _rh_i),key))) { \
                tbl##_setvalue(u, _rh_i, key, value); \
                break; \
            } \
            size_t _rh_i_want = tbl##_getbucket(u, tbl##_getkey(u, _rh_i)); \
            size_t _rh_i_temp; \
            if (_rh_i_want <= _rh_i) { \
                _rh_i_temp = _rh_i - _rh_i_want; \
            } else  { \
                _rh_i_temp = tbl##_n_elem(u) - _rh_i_want + _rh_i - 1; \
            } \
            if (_rh_i_temp < _rh_temperature) { \
                /* Rob the rich, give the poor */ \
                tbl##_swap(u, 0, _rh_i); \
                _rh_temperature = _rh_i_temp; \
                _rh_check_for_match = 0; \
            } \
            _ROBINHOOD_HASH_TYPICAL_INCREMENT({tbl##_overflow(u);}, tbl##_n_elem(u)) \
        } \
    } \
}
    
#define _ROBINHOOD_HASH_GET(tbl, u, key, assignme) { \
    _ROBINHOOD_HASH_TYPICAL_INIT(u, key, tbl##_getbucket) \
    for(;;) { \
        if (tbl##_isnil(u, _rh_i)) { \
            assignme = tbl##_nilvalue(u); \
            break; \
        } else { \
            if (tbl##_keysequal(u, tbl##_getkey(u, _rh_i),key)) { \
                assignme = tbl##_getvalue(u, _rh_i); \
                break; \
            } \
            _ROBINHOOD_HASH_TYPICAL_INCREMENT({assignme = tbl##_nilvalue(u);}, tbl##_n_elem(u)) \
        } \
    } \
}

#define _ROBINHOOD_HASH_DEL(tbl, u, key) { \
    _ROBINHOOD_HASH_TYPICAL_INIT(u, key, tbl##_getbucket) \
    int _ri_needbackshift = 0; \
    for(;;) { \
        if (tbl##_isnil(u, _rh_i)) { \
            tbl##_removefailed(u, key); \
            break; \
        } else { \
            if (tbl##_keysequal(u, tbl##_getkey(u, _rh_i),key)) { \
                tbl##_setnil(u, _rh_i); \
                _ri_needbackshift = 1; \
                break; \
            } \
            _ROBINHOOD_HASH_TYPICAL_INCREMENT({tbl##_removefailed(u, key);}, tbl##_n_elem(u)) \
        } \
    } \
    /* Backshift */ \
    if(_ri_needbackshift)\
    for(;;) { \
        size_t _rh_nexti = _rh_i + 1; \
        if (_rh_nexti >= tbl##_n_elem(u)) _rh_nexti=1; \
        if (tbl##_isnil(u, _rh_nexti)) { \
            break; \
        } else \
        if (tbl##_getbucket(u, tbl##_getkey(u, _rh_nexti)) == _rh_nexti) { \
            break; \
        } else { \
            tbl##_swap(u, _rh_nexti, _rh_i); \
        } \
        _rh_i = _rh_nexti; \
    } \
}
    
#define _ROBINHOOD_HASH_SIZE(tbl, u, assignme) { \
    size_t _rh_i; \
    assignme = 0; \
    for(_rh_i=0; _rh_i<tbl##_n_elem(u); ++_rh_i) { \
        if (!(tbl##_isnil(u, _rh_i))) assignme+=1; \
    } \
}

#define _ROBINHOOD_HASH_CLEAR(tbl, u) { \
    size_t _rh_i; \
    for(_rh_i=0; _rh_i<tbl##_n_elem(u); ++_rh_i) { \
        tbl##_setnil(u, _rh_i) \
    } \
}
    
#endif // ROBINHOOD_HASH_H
