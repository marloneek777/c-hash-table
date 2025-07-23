#include<stdlib.h>
#include<string.h>
#include<cmath>

#include"hash-table.h"

static const int HT_PRIME_1 = 151;
static const int HT_PRIME_2 = 163;

static ht_item* ht_new_item(const char* k, const char* v)
{
  ht_item* i = (ht_item*)malloc(sizeof(ht_item));
  i->key = strdup(k);
  i->value = strdup(v);
  return i;
}

ht_hash_table* ht_new()
{
  ht_hash_table* table = (ht_hash_table*)malloc(sizeof(ht_hash_table));
  table->size = 53;
  table->count = 0;
  table->items = (ht_item**)calloc((size_t)table->size, sizeof(ht_item*));
  return table;
}

void free_ht_item(ht_item* i)
{
  free(i->key);
  free(i->value);
  free(i);
}

void free_ht(ht_hash_table* ht)
{
  for ( size_t i = 0; i < ht->size; ++i )
  {
    ht_item* item = ht->items[i];
    if ( item != nullptr )
    {
      free_ht_item(item);
    }
  }
  free(ht->items);
  free(ht);
}

static int ht_hash(const char* s, const int a, unsigned int m)
{
  long hash = 0;
  const int s_len = strlen(s);
  for (int i = 0; i < s_len; i++)
  {
    hash += (long)pow(a, s_len - (i+1)) * s[i];
    hash = hash % m;
  }
  return hash; 
}

static int ht_get_hash(const char* s, const int num_buckets, const int attempt)
{
  int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
  return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

