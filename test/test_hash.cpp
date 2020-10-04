#include <catch.hpp>

#include <kb/hash.h>

#include <kb/crt.h>

TEST_CASE("hashgen should return same value for same string", "[hash]") {
  kb_hash a = kb_hash_string("asdfghjkl");
  kb_hash b = kb_hash_string("asdfghjkl");
  
  REQUIRE(a == b);
}

TEST_CASE("hashgen should return different value for different string", "[hash]") {
  kb_hash a = kb_hash_string("asdfghjkl;");
  kb_hash b = kb_hash_string("asdfghjkl");
  
  REQUIRE(a != b);
}

TEST_CASE("kb_hash_string and kb_hash_gen should return same hash for same data", "[hash]") {
  const char* str = "asdfghjkl";
  uint32_t len = kb_strlen(str);

  kb_hash a = kb_hash_string(str);

  kb_hash_gen gen;
  kb_hash_begin(&gen);
  kb_hash_add(&gen, str, len);
  kb_hash b = kb_hash_end(&gen);

  REQUIRE(a == b);

}
