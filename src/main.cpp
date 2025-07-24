#include"hash-table.h"
#include"prime.h"

int main()
{

  int last_prime = 53;
  int prime = next_prime(last_prime + 1);


  
  ht_hash_table* ht = ht_new();
  free_ht(ht);
  
  return 0;
}