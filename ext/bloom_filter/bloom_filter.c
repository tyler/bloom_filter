#include "bloom.h"
#include "ruby.h"
#include <stdlib.h>

VALUE cBloomFilter;

void rb_bloom_free(void *bloom) {
	bloom_free(bloom);
}

static VALUE rb_bloom_alloc(VALUE klass) {
	VALUE obj = Data_Wrap_Struct(klass, 0, rb_bloom_free, bloom_alloc());
	return obj;
}

Bloom *get_bloom(VALUE self) {
	Bloom *filter;
	Data_Get_Struct(self, Bloom, filter);
	return filter;
}

static VALUE rb_bloom_initialize(VALUE self, VALUE bitset_size, VALUE hash_count) {
	Bloom *filter = get_bloom(self);
	bloom_set_bitset(filter, NUM2INT(bitset_size));
	bloom_set_hashes(filter, NUM2INT(hash_count));
	return self;
}

static VALUE rb_bloom_set(VALUE self, VALUE key) {
	Bloom *filter = get_bloom(self);
	int hash_key = NUM2INT(rb_hash(key));
	bloom_add(filter, &hash_key, sizeof(int));
	return Qtrue;
}

static VALUE rb_bloom_get(VALUE self, VALUE key) {
	Bloom *filter = get_bloom(self);
	int hash_key = NUM2INT(rb_hash(key));
	return bloom_get(filter, &hash_key, sizeof(int)) ? Qtrue : Qnil;
}

static VALUE rb_bloom_filter_size(VALUE self) {
	Bloom *filter = get_bloom(self);
	return INT2FIX(filter->bitset_size);
}

static VALUE rb_bloom_hash_count(VALUE self) {
	Bloom *filter = get_bloom(self);
	return INT2FIX(filter->hash_count);
}

static VALUE rb_bloom_for_error_rate(VALUE self, VALUE error_rate, VALUE key_count) {
	VALUE rb_filter = rb_bloom_alloc(cBloomFilter);
	Bloom *filter = get_bloom(rb_filter);

	int size = bloom_size_for_error(NUM2DBL(error_rate), NUM2INT(key_count));
	int hash_count = bloom_ideal_hash_count(size, NUM2INT(key_count));

	bloom_set_bitset(filter, size);
	bloom_set_hashes(filter, hash_count);

	return rb_filter;
}

void Init_bloom_filter() {
    cBloomFilter = rb_define_class("BloomFilter", rb_cObject);
    rb_define_alloc_func(cBloomFilter, rb_bloom_alloc);
	rb_define_method(cBloomFilter, "initialize", rb_bloom_initialize, 2);
    rb_define_method(cBloomFilter, "get", rb_bloom_get, 1);
    rb_define_method(cBloomFilter, "set", rb_bloom_set, 1);
    rb_define_method(cBloomFilter, "filter_size", rb_bloom_filter_size, 0);
    rb_define_method(cBloomFilter, "hash_count", rb_bloom_hash_count, 0);
	rb_define_singleton_method(cBloomFilter, "for_error_rate", rb_bloom_for_error_rate, 2);
}
