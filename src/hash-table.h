#pragma once

struct ht_item 
{
  char* key;
  char* value;
};

struct ht_hash_table
{
  int size;
  int count;
  ht_item** items;
};

ht_hash_table* ht_new();
void ht_delete( ht_hash_table* ht, const char* key );
void ht_insert( ht_hash_table* ht, const char* key, const char* value );
char* ht_search( ht_hash_table* ht, const char* key );
void free_ht( ht_hash_table* ht );