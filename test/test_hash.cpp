#include <catch.hpp>

#include <kb/hash.h>

#include <kb/crt.h>

TEST_CASE("hashgen should return same value for same string", "[hash]") {
  Hash a = kb_hash_string("asdfghjkl");
  Hash b = kb_hash_string("asdfghjkl");
  
  REQUIRE(a == b);
}

TEST_CASE("hashgen should return different value for different string", "[hash]") {
  Hash a = kb_hash_string("asdfghjkl;");
  Hash b = kb_hash_string("asdfghjkl");
  
  REQUIRE(a != b);
}

TEST_CASE("kb_hash_string, kb_hash_memory and HashGen should return same hash for same data", "[hash]") {
  const char* str = "asdfghjkl";
  uint32_t len = kb_strlen(str);

  Hash a = kb_hash_string(str);

  HashGen gen;
  kb_hash_begin(&gen);
  kb_hash_add(&gen, str, len);
  Hash b = kb_hash_end(&gen);

  Memory mem;
  mem.data = str;
  mem.size = len;
  Hash c = kb_hash_memory(mem);

  REQUIRE(a == b);
  REQUIRE(b == c);
  REQUIRE(c == a);
}
