#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bloom.h"

#define TRUE 1
#define FALSE 0
#define jen_mix(a,b,c) { \
  a -= b; a -= c; a ^= ( c >> 13 ); \
  b -= c; b -= a; b ^= ( a << 8 ); \
  c -= a; c -= b; c ^= ( b >> 13 ); \
  a -= b; a -= c; a ^= ( c >> 12 ); \
  b -= c; b -= a; b ^= ( a << 16 ); \
  c -= a; c -= b; c ^= ( b >> 5 ); \
  a -= b; a -= c; a ^= ( c >> 3 ); \
  b -= c; b -= a; b ^= ( a << 10 ); \
  c -= a; c -= b; c ^= ( b >> 15 ); \
}

#define bloom_index_jen(filter,key,len,salt_idx) \
	bloom_hash_jen((void*)key, len, *(filter->salts + salt_idx)) % filter->bitset_size;


Bloom *bloom_alloc() {
	return (Bloom*)(malloc(sizeof(Bloom)));
}

void bloom_set_hashes(Bloom* filter, int hash_count) {
	filter->hash_count = hash_count;
	filter->salts = (unsigned int*)(malloc(sizeof(int) * hash_count));
	int i;
	for(i = 0; i < hash_count; i++)
		*(filter->salts + i) = (unsigned int)rand();
}

void bloom_set_bitset(Bloom* filter, int bitset_size) {
	filter->bitset_size = bitset_size;
	filter->bitset = (bitstr_t*)(malloc(bitstr_size(bitset_size)));
}

Bloom *bloom_new(int bitset_size, int hash_count) {
	Bloom *filter = bloom_alloc();
	bloom_set_bitset(filter, bitset_size);
	bloom_set_hashes(filter, hash_count);
	return filter;
}

int bloom_size_for_error(double error, int key_count) {
	return ceil((key_count * log(error)) / log(1.0 / (pow(2.0, log(2.0)))));
}

int bloom_ideal_hash_count(int size, int key_count) {
	return round(log(2.0) * size / key_count);
}

Bloom *bloom_for_error_and_keys(double error, int key_count) {
	int size = bloom_size_for_error(error, key_count);
	int hashes = bloom_ideal_hash_count(size, key_count);
	return bloom_alloc(size, hashes);
}

void bloom_free(Bloom *filter) {
	free(filter->bitset);
	free(filter);
}


void bloom_add(Bloom *filter, void *key, unsigned int key_size) {
	int i;
	for(i = 0; i < filter->hash_count; i++) {
		int bit = bloom_index_jen(filter, key, key_size, i);
		bit_set(filter->bitset, bit);
	}
}

int bloom_get(Bloom *filter, void *key, unsigned int key_size) {
	int i;
	for(i = 0; i < filter->hash_count; i++) {
		int bit = bloom_index_jen(filter, key, key_size, i);
		if(bit_test(filter->bitset, bit) == 0)
			return FALSE;
	}

	return TRUE;
}

unsigned int bloom_hash_jen (void *key, unsigned int length, unsigned int salt) {
	unsigned char *k = (unsigned char*) key;
	unsigned a, b;
	unsigned c = salt;
	unsigned int len = length;

	a = b = 0x9e3779b9;

	while ( len >= 12 ) {
		a += ( k[0] + ( (unsigned)k[1] << 8 ) 
			   + ( (unsigned)k[2] << 16 )
			   + ( (unsigned)k[3] << 24 ) );
		b += ( k[4] + ( (unsigned)k[5] << 8 ) 
			   + ( (unsigned)k[6] << 16 )
			   + ( (unsigned)k[7] << 24 ) );
		c += ( k[8] + ( (unsigned)k[9] << 8 ) 
			   + ( (unsigned)k[10] << 16 )
			   + ( (unsigned)k[11] << 24 ) );

		jen_mix ( a, b, c );

		k += 12;
		len -= 12;
	}

	c += length;

	switch ( len ) {
	case 11: c += ( (unsigned)k[10] << 24 );
	case 10: c += ( (unsigned)k[9] << 16 );
	case 9 : c += ( (unsigned)k[8] << 8 );
		/* First byte of c reserved for length */
	case 8 : b += ( (unsigned)k[7] << 24 );
	case 7 : b += ( (unsigned)k[6] << 16 );
	case 6 : b += ( (unsigned)k[5] << 8 );
	case 5 : b += k[4];
	case 4 : a += ( (unsigned)k[3] << 24 );
	case 3 : a += ( (unsigned)k[2] << 16 );
	case 2 : a += ( (unsigned)k[1] << 8 );
	case 1 : a += k[0];
	}

	jen_mix ( a, b, c );

	return c;
}

void bloom_status(Bloom *filter) {
	printf("Bitset Size: %d\n", filter->bitset_size);
	printf("Hash Count: %d\n", filter->hash_count);
}


