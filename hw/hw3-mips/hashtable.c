// CS250P Homework 3
//
// A really naive hash table with linear probing.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <papi.h>

#define KEY_SIZE 24
#define TABLE_SIZE 1000000

uint64_t rd_state = 14695981039346656037lu;
uint8_t rd_counter = 0;

// The FNV hash function, used here as a deterministic pseudo-random number generator.
uint64_t pseudo_random() {
	rd_state *= 1099511628211;
	rd_state ^= rd_counter++;
	return rd_state;
}

typedef struct {
	uint8_t key[KEY_SIZE]; // not a C string - we always compare all 24 bytes
	uint64_t value;
} kv;

typedef struct {
	kv entry[TABLE_SIZE];
} hashmap;

void key_generate(uint8_t *dest) {
	for (size_t i = 0; i < KEY_SIZE; ++i) {
		// shift some bits for more "randomness"
		dest[i] = (pseudo_random() >> i) & 0xff;
	}
}

uint64_t key_hash(uint8_t *key) {
	uint64_t hash = 14695981039346656037lu;

	for (size_t i = 0; i < KEY_SIZE; ++i) {
		hash *= 1099511628211;
		hash ^= key[i];
	}

	return hash;
}

bool key_compare(uint8_t *a, uint8_t *b) {
	for (size_t i = 0; i < KEY_SIZE; ++i) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	
	return true;
}

hashmap* hashmap_init() {
	hashmap *map = malloc(sizeof(hashmap));

	for (size_t i = 0; i < TABLE_SIZE; ++i) {
		map->entry[i].value = 0;
	}

	return map;
}

uint64_t hashmap_lookup(hashmap *map, uint8_t *key) {
	uint64_t hash = key_hash(key);

	for (size_t i = 0; i < TABLE_SIZE; ++i) {
		size_t idx = (hash + i) % TABLE_SIZE;

		if (map->entry[idx].value == 0) {
			return 0;
		}
		if (key_compare(map->entry[idx].key, key)) {
			return map->entry[idx].value;
		}
	}

	return 0;
}

bool hashmap_insert(hashmap *map, uint8_t *key, uint64_t value) {
	assert(value);
	uint64_t hash = key_hash(key);

	for (size_t i = 0; i < TABLE_SIZE; ++i) {
		size_t idx = (hash + i) % TABLE_SIZE;

		if (map->entry[idx].value == 0) {
			memcpy(map->entry[idx].key, key, KEY_SIZE);
			map->entry[idx].value = value;
			return true;
		}

		assert(!key_compare(map->entry[idx].key, key));
	}

	return false;
}

int main(int argc, char **argv) {
	hashmap *map = hashmap_init();

	size_t size = 1000;
	uint64_t sum = 0;

	uint8_t (*keys)[KEY_SIZE] = malloc(size * KEY_SIZE);
	for (size_t i = 0; i < size; ++i) {
		key_generate(keys[i]);
		assert(hashmap_insert(map, keys[i], i + 1));
	}
	printf("Inserted %lu entries\n", size);

	// Workload starts here
	for (size_t i = 0; i < size; ++i) {
		size_t ki = pseudo_random() % size;
		sum += hashmap_lookup(map, keys[ki]);
	}
	printf("Random sum: %lu\n", sum);

	return 0;
}
