#include"hash-table.h"

int main()
{
  ht_hash_table* ht = ht_new();
  free_ht(ht);

  int x = ht_hash("killimanjarooek", 142, 53);
  
  return 0;
}