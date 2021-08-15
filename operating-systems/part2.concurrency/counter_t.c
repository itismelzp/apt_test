//
// Created by markrenChina on 2021/8/7.
// VCPKG_VISUAL_STUDIO_PATH
//

#include <pthread.h>

#define NUMCPUS 6
/**
 * 懒惰计数器
 */
typedef struct counter_t {
    int               global;                  //global count
    pthread_mutex_t   glock;                   //global lock
    int               local[NUMCPUS];           //local count ( per cpu )
    pthread_mutex_t   llocak[NUMCPUS];         //... and locks
    int               threshold;               //update frequency
} counter_t;

//init:record threshold, init locks,init values
//       of all local counts and global count
void init(counter_t* c, int threshold) {
    c->threshold = threshold;

    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);

    int i;
    for (i = 0; i < NUMCPUS; i++) {
        c->local[i] = 0;
        pthread_mutex_init(&c->llocak[i], NULL);
    }
}

//update : usually,just grab local lock and update local amount
//         once local count has risen by 'threshold',grab global
//         lock and transfer local values to it
void update(counter_t* c, int threadID, int amt) {
    pthread_mutex_lock(&c->llocak[threadID]);
    c->local[threadID] += amt;                         //assume amt >0
    if (c->local[threadID] >= c->threshold) 
    {                                                  
        // transfer to global
        pthread_mutex_lock(&c->glock);
        c->global += c->local[threadID];
        pthread_mutex_unlock(&c->global);
        c->local[threadID] = 0;
    }
    pthread_mutex_unlock(&c->llocak[threadID]);
}

//get: just return global amout (which may not be perfect)
int get(counter_t *c) {
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val;                                         //only approximate!
}