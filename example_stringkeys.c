#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "robinhoodhash.h"

struct entry {
    char key[10];
    int value;
};

struct entry hshtable [100];

#define strk_setvalue(u, index, key_, val_) \
        strncpy(u[index].key, key_, 10); \
        u[index].value=val_;
#define strk_setnil(u, index)   \
        u[index].key[0]='\0'; u[index].value=0;
#define strk_swap(u, index1, index2) \
        struct entry tmp = u[index1]; \
        u[index1] = u[index2]; \
        u[index2] = tmp;

unsigned short int simplehash(const char* x) {
    unsigned short int q = 0x1665;
    while(*x) {
        q *= 37;
        q ^= (unsigned short)(unsigned char)(*x);
        ++x;
    }
    return q;
}

#define strk_nilvalue(u)          0
#define strk_getvalue(u,index)    u[index].value
#define strk_getkey(u,index)      u[index].key
#define strk_keysequal(u,key1,key2) !strncmp(key1, key2, 10)
#define strk_isnil(u,index)       u[index].key[0] == '\0'
#define strk_n_elem(u)            (sizeof(u)/sizeof(*u))
#define strk_getbucket(u,key)     simplehash(key)%((sizeof(u)/sizeof(*u)) - 1) + 1
#define strk_overflow(u)          {}
#define strk_removefailed(u,key)  {}

int main() {
    ROBINHOOD_HASH_CLEAR(strk, hshtable);
    
    int value = '_';
    
    ROBINHOOD_HASH_GET(strk, hshtable, "4433", value);
    assert(value == 0);
    
    ROBINHOOD_HASH_SET(strk, hshtable, "4433", 'A');
    ROBINHOOD_HASH_SET(strk, hshtable, "123",  'B');
    
    ROBINHOOD_HASH_GET(strk, hshtable, "4433", value);
    assert(value == 'A');
    ROBINHOOD_HASH_GET(strk, hshtable, "123", value);
    assert(value == 'B');
    
    ROBINHOOD_HASH_DEL(strk, hshtable, "4433");
    
    ROBINHOOD_HASH_GET(strk, hshtable, "4433", value);
    assert(value == 0);
    ROBINHOOD_HASH_GET(strk, hshtable, "123", value);
    assert(value == 'B');
    return 0;
}
