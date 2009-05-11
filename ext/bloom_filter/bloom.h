#include <bitstring.h>

typedef struct {
	bitstr_t *bitset;
	int bitset_size;
	int hash_count;
	unsigned int *salts;
} Bloom;

Bloom *bloom_alloc();
Bloom *bloom_new(int bitset_size, int hash_count);
void bloom_set_hashes(Bloom* filter, int hash_count);
void bloom_set_bitset(Bloom* filter, int hash_count);
Bloom *bloom_for_error_and_keys(double error, int key_count);
void bloom_free(Bloom *filter);
void bloom_add(Bloom *filter, void *key, unsigned int key_size);
int bloom_get(Bloom *filter, void *key, unsigned int key_size);
unsigned int bloom_hash_jen(void *key, unsigned int length, unsigned int salt);
int bloom_size_for_error(double error, int key_count);
int bloom_ideal_hash_count(int size, int key_count);
