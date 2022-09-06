#include "bits.h"
#include "cache.h"
#include "math.h"

int get_set(Cache *cache, address_type address) {
  // TODO:
  //  Extract the set bits from a 32-bit address.
  //
  unsigned int mask = 0;
  unsigned int ans = 0;

  for (int i = 0; i < cache->set_bits; i++) {
    mask = mask + exp2(i);
  }
  mask = mask << cache->block_bits;

  ans = mask & address;
  ans = ans >> cache->block_bits;
  
  return ans;
}

int get_line(Cache *cache, address_type address) {
  // TODO:
  // Extract the tag bits from a 32-bit address.
  //
  unsigned int mask = 0xFFFFFFFF;
  unsigned int offset = cache->block_bits + cache->set_bits;
  unsigned int ans = 0;

  mask = mask << offset;
  ans = mask & address;
  ans = ans >> offset;

  return ans;
}

int get_byte(Cache *cache, address_type address) {
  // TODO
  // Extract the block offset (byte index) bits from a 32-bit address.
  //
  unsigned int mask = 0;
  unsigned int ans = 0;

  for (int i = 0; i < cache->block_bits; i++) {
    mask = mask + exp2(i);
  }

  ans = mask & address;

  return ans;
}
