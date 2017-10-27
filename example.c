#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "robinhoodhash.h"

struct entry {
    int key;
    char value;
};

struct entry hshtable [100];

#define qqq_setvalue(u, index, key_, val_) \
        u[index].key=key_; u[index].value=val_;
#define qqq_setnil(u, index)   \
        u[index].key=-1; u[index].value=0;
#define qqq_swap(u, index1, index2) \
        struct entry tmp = u[index1]; \
        u[index1] = u[index2]; \
        u[index2] = tmp;

#define qqq_nilvalue(u)          0
#define qqq_getvalue(u,index)    u[index].value
#define qqq_getkey(u,index)      u[index].key
#define qqq_keysequal(u,key1,key2) key1 == key2
#define qqq_isnil(u,index)       u[index].key == -1
#define qqq_n_elem(u)            (sizeof(u)/sizeof(*u))
#define qqq_getbucket(u,key)     key%99 + 1
#define qqq_overflow(u)          {}
#define qqq_removefailed(u,key)  {}

int main() {
    ROBINHOOD_HASH_CLEAR(qqq, hshtable);
    
    char value = '_';
    
    ROBINHOOD_HASH_GET(qqq, hshtable, 4433, value);
    assert(value == 0);
    
    ROBINHOOD_HASH_SET(qqq, hshtable, 4433, 'A');
    ROBINHOOD_HASH_SET(qqq, hshtable, 123,  'B');
    
    ROBINHOOD_HASH_GET(qqq, hshtable, 4433, value);
    assert(value == 'A');
    ROBINHOOD_HASH_GET(qqq, hshtable, 123, value);
    assert(value == 'B');
    
    ROBINHOOD_HASH_DEL(qqq, hshtable, 4433);
    
    ROBINHOOD_HASH_GET(qqq, hshtable, 4433, value);
    assert(value == 0);
    ROBINHOOD_HASH_GET(qqq, hshtable, 123, value);
    assert(value == 'B');
    return 0;
}
