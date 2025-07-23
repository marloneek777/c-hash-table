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
int ht_hash(const char* s, const int a, unsigned int m);
void free_ht(ht_hash_table* ht);