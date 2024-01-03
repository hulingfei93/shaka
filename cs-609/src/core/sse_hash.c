#include <sse_core.h>

static sse_uint_t sse_hash_func(char *str, sse_uint_t len);

sse_hash_t *
sse_hash_create(size_t size, sse_pool_t *pool, sse_log_t *log)
{
	sse_hash_t *hash_table;

	hash_table = (sse_hash_t *)sse_pool_alloc(pool, sizeof(sse_hash_t));
	if (!hash_table) {
		return SSE_NULL;
	}

	hash_table->buckets = (sse_hash_kv_t **)sse_pool_calloc(pool, size * sizeof(sse_hash_kv_t *));
	if (hash_table->buckets == SSE_NULL) {
		return SSE_NULL; 
	}

	hash_table->bucket_size = size;
	hash_table->kv_count = 0;
	hash_table->conflict_count = 0;
	hash_table->pool = pool;
	hash_table->log = log;

	return hash_table;
}

sse_bool_t
sse_hash_contains(sse_hash_t *hash_table, sse_str_t *key)
{
	sse_uint_t key_hash, bucket_index;
	sse_hash_kv_t *bucket;
	sse_bool_t result;

	if (!key) {
		return SSE_BOOL_FALSE;
	}

	key_hash = sse_hash_func(key->data, key->len);
	bucket_index = key_hash % hash_table->bucket_size;
	bucket = hash_table->buckets[bucket_index];
	
	result = SSE_BOOL_FALSE;

	while (bucket) {
		if (sse_str_strcmp(bucket->key, key) == 0) {
			return SSE_BOOL_TRUE;
		}
		bucket = bucket->next;
	}

	return result;
}

sse_bool_t
sse_hash_add(sse_hash_t *hash_table, sse_str_t *key, void *value)
{
	sse_bool_t result;
	sse_uint_t key_hash, bucket_index;
	sse_hash_kv_t *bucket, *kv, *new_kv;


	if (!key) {
		return SSE_BOOL_FALSE;
	}
	
	key_hash = sse_hash_func(key->data, key->len);
	bucket_index = key_hash % hash_table->bucket_size;
	bucket = hash_table->buckets[bucket_index];
	kv = bucket;

	result = SSE_BOOL_TRUE;
	
	/* find if the same key exists in the chain */
	while (kv) {
		if (sse_str_strcmp(kv->key, key) == 0) {
			return SSE_BOOL_FALSE;
		}
		kv = kv->next;
	}

	if (kv != bucket) {
		++hash_table->conflict_count;
	}

	/* no same key exists, so add safty */
	new_kv = (sse_hash_kv_t *)sse_pool_alloc(hash_table->pool, sizeof(sse_hash_kv_t));
	new_kv->key = key;
	new_kv->value = value;

	/* insert the new hash kv into the head of chain */
	new_kv->next = bucket;
	hash_table->buckets[bucket_index] = new_kv;
	
	++hash_table->kv_count;

	return result;
}

void *
sse_hash_find(sse_hash_t *hash_table, sse_str_t *key)
{
	sse_uint_t key_hash, bucket_index;
	sse_hash_kv_t *bucket;

	if (!key) {
		return SSE_NULL;
	}

	key_hash = sse_hash_func(key->data, key->len);
	bucket_index = key_hash % hash_table->bucket_size;
	bucket = hash_table->buckets[bucket_index];

	while (bucket) {
		if (sse_str_strcmp(bucket->key, key) == 0) {
			return bucket->value;
		}
		bucket = bucket->next;
	}

	return SSE_NULL;
}

void
sse_hash_debug_info(sse_hash_t *hash_table)
{
	printf("*******hash summary**********\n");
	printf("bucket size: %d\n", (sse_int_t)(hash_table->bucket_size));
	printf("total count: %u\n", hash_table->kv_count);
	printf("conflict count: %u\n", hash_table->conflict_count);
	if (hash_table->kv_count > 0) {
		printf("conflict ratio: %f\n", (double)(hash_table->conflict_count) / hash_table->kv_count);
	}

}

static sse_uint_t
sse_hash_func(char *str, sse_uint_t len)
{
	sse_uint_t hash = 5381;
	sse_uint_t i = 0;

	for (i = 0; i < len; ++i, ++str) {
		hash = ((hash << 5) + hash) + (*str);
	}

	return hash;
}
