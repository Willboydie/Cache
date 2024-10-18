#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define cache_size 10 //doesn't work for cache_size < 2 (requires some code to deal with 0,1 cases)

typedef struct entry{

    char *key;
    int value;

    struct entry *next_in_cache;
    struct entry *prev_in_cache;
    struct entry *next_link;
    struct entry *prev_link;

} entry;


typedef struct cache{

    struct entry **entries;
    int num_elements;

    struct entry * head;
    struct entry * tail;

} cache;


unsigned long hashfunc(char *str){

    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = hash * 33 + c;

    return hash % cache_size;
}

cache *init_cache(){
    
    cache *cache_p = malloc(sizeof(*cache_p));
    cache_p->entries = calloc(sizeof(entry*), cache_size);
    cache_p->head = NULL;
    cache_p->tail = NULL;
    cache_p->num_elements = 0;
    
    return cache_p;
}

void print_hashtable(cache *cache_p){

    printf("\n\nHEAD");
    for(int i = 0; i<cache_size; i++){
        if(cache_p->entries[i] == NULL){
            printf("\n--");
        }
        else{
            entry *tmp = cache_p->entries[i];
            printf("\n");
            while(tmp != NULL){
                printf("%s - %d,   ", tmp->key, tmp->value);
                tmp = tmp->prev_link;
            }
        }
    }
    printf("\nTAIL\n");
}

entry * create_new_entry(char *new_key, int new_value){
    entry *e = malloc(sizeof(*e));
    e->value = new_value;
    e->key = new_key;

    return e;
}

void delete_tail(cache * cache_p){
    
    int tail_index  = hashfunc(cache_p->tail->key);
    
    if(strcmp(cache_p->entries[tail_index]->key, cache_p->tail->key) == 0){
        cache_p->entries[tail_index] = cache_p->tail->prev_link;
 
        if(cache_p->tail->prev_link != NULL){
            cache_p->tail->prev_link->next_link = NULL;
        }
    }
    else{
        if(cache_p->tail->next_link != NULL){
            cache_p->tail->next_link->prev_link = cache_p->tail->prev_link;
        }
        if(cache_p->tail->prev_link != NULL){
            cache_p->tail->prev_link->next_link = cache_p->tail->next_link;
        }
    }

    cache_p->tail = cache_p->tail->next_in_cache;
    cache_p->tail->prev_in_cache = NULL;

    cache_p->num_elements --;

}

void put(cache *cache_p, char *new_key, int new_value){
    
    if(cache_p->num_elements == cache_size){
            delete_tail(cache_p);
        }
    
    int index = hashfunc(new_key);

    entry *e = create_new_entry(new_key, new_value);

    e->prev_link = cache_p->entries[index];
    
    if(e->prev_link != NULL){
        e->prev_link->next_link = e;
    }

    cache_p->entries[index] = e;

    if(cache_p->num_elements > 0){
        cache_p->head->next_in_cache = e;
        e->prev_in_cache = cache_p->head;
        e->prev_in_cache->next_in_cache = e;
        
    }
    if(cache_p->num_elements == 1){
        cache_p->tail = e->prev_in_cache;
        cache_p->tail->prev_in_cache = NULL;
    }
    
    cache_p->head = e;
    e->next_in_cache = NULL;
    cache_p->num_elements++;
}

bool get(cache *cache_p, char *search_key){

    int index = hashfunc(search_key);
    entry *tmp = cache_p->entries[index];

    if(tmp == NULL){
        printf("\nThere is no matching key \"%s\".", search_key);
        return false;
    }
    while(strcmp(tmp->key, search_key) != 0){
        tmp = tmp->prev_link;
        if(tmp == NULL){
            printf("\nThere is no matching key \"%s\".", search_key);
            return false;
        }
    }

    if(tmp == cache_p->head){

    }
    else{
        if(tmp != cache_p->tail){
            tmp->prev_in_cache->next_in_cache = tmp->next_in_cache;
        }
        else{
            cache_p->tail = tmp->next_in_cache;
            cache_p->tail->prev_in_cache = NULL;
        }

        tmp->next_in_cache->prev_in_cache = tmp->prev_in_cache;
        cache_p->head->next_in_cache = tmp;
        tmp->prev_in_cache = cache_p->head;

        cache_p->head = tmp;
        tmp->next_in_cache = NULL;
    }

    printf("\n\"%s\"  =  %d", search_key, tmp->value);
    return true;
}

void print_cache(cache * cache_p){
    
    entry * tmp = malloc(sizeof(entry*));
    tmp = cache_p->tail;

    printf("\n(TAIL) - ");
    while(tmp != NULL){
        printf("%s - ", tmp->key);
        tmp = tmp->next_in_cache;
    }
    printf("(HEAD)");
}

void main(){

    cache *my_cache = init_cache();

    put(my_cache, "Hello", 4);
    put(my_cache, "Andy Muzz", 1);
    put(my_cache, "BooBoo Bby", 7);
    put(my_cache, "big dog", 8);
    put(my_cache, "dingdong", 133);
    put(my_cache, "england", 1);

    get(my_cache, "BooBoo Bby");

    put(my_cache, "bronze", 5);

    get(my_cache, "dingdong");

    print_cache(my_cache);
    print_hashtable(my_cache);
    
    put(my_cache, "bunnyrabid", 4);

    get(my_cache, "dogbone");

    print_cache(my_cache);

    get(my_cache, "BooBoo Bby");

    print_cache(my_cache);
}

