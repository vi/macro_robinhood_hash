#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "robinhoodhash.h"

struct entry {
    int key;
    char value;
};

struct entry hshtable [100];

#define qqq_setvalue(index, key_, val_) \
        hshtable[index].key=key_; hshtable[index].value=val_;
#define qqq_setnil(index)   \
        hshtable[index].key=-1; hshtable[index].value=0;
#define qqq_swap(index1, index2) \
        struct entry tmp = hshtable[index1]; \
        hshtable[index1] = hshtable[index2]; \
        hshtable[index2] = tmp;

#define qqq_nilvalue        0
#define qqq_getvalue(index) hshtable[index].value
#define qqq_getkey(index)   hshtable[index].key
#define qqq_keysequal(key1,key2) key1 == key2
#define qqq_isnil(index)    hshtable[index].key == -1
#define qqq_n_elem          100
#define qqq_getbucket(key)  key%99 + 1
#define qqq_overflow        {}
#define qqq_removefailed(key)  {}

int main() {
    ROBINHOOD_HASH_CLEAR(qqq);
    
    char value = '_';
    
    ROBINHOOD_HASH_GET(qqq, 4433, value);
    assert(value == 0);
    
    ROBINHOOD_HASH_SET(qqq, 4433, 'A');
    ROBINHOOD_HASH_SET(qqq, 123,  'B');
    
    ROBINHOOD_HASH_GET(qqq, 4433, value);
    assert(value == 'A');
    ROBINHOOD_HASH_GET(qqq, 123, value);
    assert(value == 'B');
    
    ROBINHOOD_HASH_DEL(qqq, 4433);
    
    ROBINHOOD_HASH_GET(qqq, 4433, value);
    assert(value == 0);
    ROBINHOOD_HASH_GET(qqq, 123, value);
    assert(value == 'B');
    return 0;
}
