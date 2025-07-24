#include<stdlib.h>
#include<string.h>
#include<cmath>

#include"hash-table.h"
#include"prime.h"

static const int HT_PRIME_1 = 151;
static const int HT_PRIME_2 = 163;
static const int HT_INITIAL_BASE_SIZE = 53;
static ht_item HT_DELETED_ITEM = {NULL, NULL};

static ht_item* ht_new_item(const char* k, const char* v)
{
  ht_item* i = (ht_item*)malloc(sizeof(ht_item));
  i->key = strdup(k);
  i->value = strdup(v);
  return i;
}

ht_hash_table* ht_new()
{
  return ht_new_resized(HT_INITIAL_BASE_SIZE);
}

static ht_hash_table* ht_new_resized(const int base_size)
{
  ht_hash_table* table = (ht_hash_table*)malloc(sizeof(ht_hash_table));
  table->base_size = base_size;
  
  table->size = next_prime(base_size);
  table->count = 0;

  table->items = (ht_item**)calloc((size_t)table->size, sizeof(ht_item*));
  return table;
}

static void ht_resize(ht_hash_table* ht, const int base_size)
{
  if ( base_size < HT_INITIAL_BASE_SIZE ) {
    return;
  }

  ht_hash_table* new_ht = ht_new_resized(base_size);
  for (int i = 0; i < ht->size; i++)
  {
    ht_item* item = ht->items[i];
    if ( item != nullptr && item != &HT_DELETED_ITEM )
    {
      ht_insert(new_ht, item->key, item->value);
    }
  }

  

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

//To insert a new key-value pair,
//we iterate through indexes until we find an empty bucket.
//We then insert the item into that bucket and increment
//the hash table's count attribute, to indicate a new item has been added.

void ht_insert( ht_hash_table* ht, const char* key, const char* value )
{
  ht_item* item = ht_new_item(key, value);
  int index = ht_get_hash( item->key, ht->size, 0);
  ht_item* colliding_item = ht->items[index];
  int i = 1;
  while ( colliding_item != nullptr )
  {
    if ( colliding_item != &HT_DELETED_ITEM )
    {
      if (strcmp(colliding_item->key, key) == 0) 
      {
        free_ht_item(colliding_item);
        ht->items[index] = item;
        return;
      }
    }
    index = ht_get_hash( item->key, ht->size, i);
    colliding_item = ht->items[index];
    i++;
  }
  ht->items[index] = item;
  ht->count++;
}

// Searching is similar to inserting, but at each iteration of the while loop,
// we check whether the item's key matches the key we're searching for.
// If it does, we return the item's value. If the while loop hits a NULL bucket,
// we return NULL, to indicate that no value was found.

char* ht_search( ht_hash_table* ht, const char* key )
{
  int index = ht_get_hash( key, ht->size, 0);
  ht_item* curr_item = ht->items[index];
  int i = 1;
  while ( curr_item != nullptr )
  {
    if ( curr_item != &HT_DELETED_ITEM )
    {
      if ( strcmp(curr_item->key, key) == 0 )
      {
        return curr_item->value;
      }
    }
    index = ht_get_hash( key, ht->size, i);
    curr_item = ht->items[index];
    i++;
  }
  return nullptr;
}

// Deleting from an open addressed hash table is more complicated than inserting or searching.
// The item we wish to delete may be part of a collision chain.
// Removing it from the table will break that chain,
// and will make finding items in the tail of the chain impossible.
// To solve this, instead of deleting the item, we simply mark it as deleted.
// We mark an item as deleted by replacing it with a pointer to a global
// sentinel item which represents that a bucket contains a deleted item.

void ht_delete( ht_hash_table* ht, const char* key )
{
  int index = ht_get_hash( key, ht->size, 0);
  ht_item* curr_item = ht->items[index];
  int i = 1;
  while ( curr_item != nullptr )
  {
    if ( curr_item != &HT_DELETED_ITEM)
    {
      if ( strcmp(curr_item->key, key) == 0 )
      {
        free_ht_item(curr_item);
        ht->items[index] = &HT_DELETED_ITEM;
        ht->count--;
        break;
      }
    }
    index = ht_get_hash( key, ht->size, i);
    curr_item = ht->items[index];
    i++;
  }
}


