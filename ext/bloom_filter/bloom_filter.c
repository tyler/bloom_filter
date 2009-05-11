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

void Init_bloom_filter() {
    cBloomFilter = rb_define_class("BloomFilter", rb_cObject);
    rb_define_alloc_func(cBloomFilter, rb_bloom_alloc);
	rb_define_method(cBloomFilter, "initialize", rb_bloom_initialize, 2);
    rb_define_method(cBloomFilter, "get", rb_bloom_get, 1);
    rb_define_method(cBloomFilter, "set", rb_bloom_set, 1);
}
