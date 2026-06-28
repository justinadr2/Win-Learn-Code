#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1024

unsigned int hash(const char* str) {
    unsigned int h = 5381;
    int c;
    
    while ((c = *str++))
        h = ((h << 5) + h) + c;
    return h;
}


typedef struct Node {
    char* key;
    int value;
    struct Node* next;
} Node;

typedef struct HashMap {
    Node* buckets[TABLE_SIZE];
} HashMap;

HashMap* hashmap_create(void) {
    HashMap* map = calloc(1, sizeof(HashMap));
    return map;
}

void hashmap_put(HashMap* map, const char* key, int value) {
    unsigned int idx = hash(key) % TABLE_SIZE;
    
    Node* node = map->buckets[idx];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            return;
        }
        node = node->next;
    }

    Node* new = malloc(sizeof(Node));
    new->key = _strdup(key);
    new->value = value;
    new->next = map->buckets[idx];
    map->buckets[idx] =  new;
}

int hashmap_get(HashMap* map, const char* key, int* out) {
    unsigned int idx = hash(key) % TABLE_SIZE;
    Node* node = map->buckets[idx];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            *out = node->value;
            return 1;
        }
        node = node->next;
    }
    return 0;
}

void hashmap_remove(HashMap* map, const char* key) {
    unsigned int idx = hash(key) % TABLE_SIZE;
    Node** pp = &map->buckets[idx];
    
    while (*pp) {
        if (strcmp((*pp)->key, key) == 0) {
            Node* tmp = *pp;
            *pp = tmp->next;
            free(tmp->key);
            free(tmp);
            return;
        }
        pp = &(*pp)->next;
    }
}

void hashmap_free(HashMap* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* node = map->buckets[i];
        while (node) {
            Node* next = node->next;
            free(node->key);
            free(node);
            node = next;
        }
    }
    free(map);
}

int main() {
    HashMap* map = hashmap_create();

    hashmap_put(map, "one", 1);
    hashmap_put(map, "two", 2);
    hashmap_put(map, "three", 3);
    
    int value;
    if (hashmap_get(map, "one", &value))
        printf("one = %d\n", value);
    
    hashmap_put(map, "one", 3);
        
    if (hashmap_get(map, "one", &value))
        printf("one = %d\n", value);

    hashmap_remove(map, "one");
        
    if (hashmap_get(map, "one", &value))
        printf("one = %d\n", value);
        
    hashmap_free(map);
}