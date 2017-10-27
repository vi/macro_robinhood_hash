#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <set>

#include "robinhoodhash.h"

#define TS 333

uint32_t hashtable[TS] = {0};

#define mytable_setvalue(u, index, key, val) hashtable[index] = val;
#define mytable_setnil(u, index) hashtable[index] = 0;
#define mytable_nilvalue(u) 0
#define mytable_getvalue(u, index) hashtable[index]
#define mytable_getkey(u, index) hashtable[index]
#define mytable_keysequal(u,key1, key2) key1 == key2
#define mytable_isnil(u,index) (hashtable[index]==0)
#define mytable_n_elem(u) TS
#define mytable_getbucket(u,key) (size_t)((key-1)%(TS-1) + 1)
#define mytable_overflow(u) printf("\nX HT full!\n");
#define mytable_removefailed(u,key) //printf("\nX Remove failed %u!\n", key);
#define mytable_swap(u,index1, index2) { \
    uint32_t tmp = hashtable[index1]; \
    hashtable[index1] = hashtable[index2]; \
    hashtable[index2] = tmp; \
}

std::set<uint32_t> theset;

int main() {
    //srand((unsigned)time(0));
    srand(23);
    
    int timetocheck = 0;
    for(;;) {
        int op = (unsigned)rand() % 10;
        uint32_t val = (unsigned)rand() % (TS*5) + 1;
        //printf("size=%d\n", theset.size());
        
        if (op == 0) {
            if (theset.size() >= TS-1) continue;
            
            uint32_t assgn = 0xFFFF;
            ROBINHOOD_HASH_GET(mytable, u, val, assgn);
            if (assgn != (theset.count(val) ? val : 0)) {
                printf("\nX Before insert element: %u\n", assgn);
            }
            // set
            
            //printf(".");
            
            ROBINHOOD_HASH_SET(mytable, u, val, val);
            
            ROBINHOOD_HASH_GET(mytable, u, val, assgn);
            if (assgn != val) {
                printf("\nX After insert element: %u instead of %u\n", assgn, val);
            }
            
            theset.insert(val);
        } else
        if (op >= 1) {
            // del
            //if (theset.count(val) == 0) continue;  
            uint32_t assgn = 0xFFFF;
            ROBINHOOD_HASH_GET(mytable, u, val, assgn);
            if (assgn != (theset.count(val) ? val : 0)) {
                printf("\nX Before removal element: %u instead of %u\n", assgn, val);
            }
        
            if (theset.count(val)) {
                // printf("-");
            }
            ROBINHOOD_HASH_DEL(mytable, u, val);
            theset.erase(val);
            
            ROBINHOOD_HASH_GET(mytable, u, val, assgn);
            if (assgn != 0) {
                printf("\nX After removal element: %u\n", assgn);
            }
            
        }
        
        //_ROBINHOOD_HASH_DEBUGPRINT;
        
        if (timetocheck<=0) {
            // check entire table for consistency
            for(uint32_t i : theset) {
                uint32_t assgn = 44444;
                ROBINHOOD_HASH_GET(mytable, u, i, assgn);
                
                if (assgn == i) {
                    // OK
                } else 
                if (assgn == 0) {
                    printf("\nX Missing element: %u\n", i);
                } else {
                    printf("\nX Error assgn=%u instead of %u\n", assgn, i);
                }
            }
            
            for(uint32_t i = 1; i<TS; ++i) {
                if (theset.count(i) > 0) continue;
                uint32_t assgn;
                ROBINHOOD_HASH_GET(mytable, u, i, assgn);
                if (assgn == 0) {
                    // OK
                } else {
                    printf("\nX Extra element: %u %u\n", i, assgn);
                }
            }
            timetocheck = 5000;
        } else {
            timetocheck -= 1;
        }
    }
    
    return 0;
}
