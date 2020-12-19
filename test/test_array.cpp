#include <catch.hpp>

#include <kb/array.h>

TEST_CASE("zero initialized array should be empty and not freak out", "[array]") {
  kb_array arr {};
  
  REQUIRE(kb_array_count    (&arr)    == 0);
  REQUIRE(kb_array_capacity (&arr)    == 0);
  REQUIRE(kb_array_get      (&arr, 5) == nullptr);
}

TEST_CASE("initialized array should have correct capacity and count", "[array]") {
  kb_array arr {};
  
  kb_array_create(&arr, sizeof(uint32_t), 10);

  REQUIRE(arr.cap == 10);
  REQUIRE(kb_array_capacity(&arr) == 10);
  
  REQUIRE(arr.pos == 0);
  REQUIRE(kb_array_count(&arr) == 0);
}

TEST_CASE("array reserve should be able to increase capacity", "[array]") {
  kb_array arr {};
  
  kb_array_create(&arr, sizeof(uint32_t), 10);
  
  CHECK(kb_array_capacity(&arr) == 10);

  kb_array_reserve(&arr, 100);

  REQUIRE(kb_array_capacity(&arr) >= 100);
}

TEST_CASE("array reserve should not be able to decrease capacity", "[array]") {
  kb_array arr {};
  
  kb_array_create(&arr, sizeof(uint32_t), 100);
  
  CHECK(kb_array_capacity(&arr) == 100);

  kb_array_reserve(&arr, 50);

  REQUIRE(kb_array_capacity(&arr) == 100);
}

TEST_CASE("array reset should not change the capacity", "[array]") {
  kb_array arr {};
  
  kb_array_create(&arr, sizeof(uint32_t), 100);
  
  CHECK(kb_array_capacity(&arr) == 100);

  kb_array_reset(&arr);

  REQUIRE(kb_array_capacity(&arr) == 100);
}

TEST_CASE("push_back should increase capacity and count", "[array]") {
  kb_array arr {};
  
  uint32_t d = 14;

  kb_array_create(&arr, sizeof(uint32_t), 0);
  
  kb_array_push_back(&arr, &d);
  REQUIRE(kb_array_capacity(&arr) >= 1);
  REQUIRE(kb_array_count(&arr) == 1);
  
  kb_array_push_back(&arr, &d);
  REQUIRE(kb_array_capacity(&arr) >= 2);
  REQUIRE(kb_array_count(&arr) == 2);

  kb_array_push_back(&arr, &d);
  REQUIRE(kb_array_capacity(&arr) >= 3);
  REQUIRE(kb_array_count(&arr) == 3);

  kb_array_push_back(&arr, &d);
  REQUIRE(kb_array_capacity(&arr) >= 4);
  REQUIRE(kb_array_count(&arr) == 4);
}

TEST_CASE("pop_back should decrease count, but not capacity", "[array]") {
  kb_array arr {};
  
  uint32_t d = 14;

  kb_array_create(&arr, sizeof(uint32_t), 0);
  
  kb_array_push_back(&arr, &d);
  kb_array_push_back(&arr, &d);
  kb_array_push_back(&arr, &d);
  kb_array_push_back(&arr, &d);
  
  REQUIRE(kb_array_capacity(&arr) >= 4);
  REQUIRE(kb_array_count(&arr) == 4);
  
  kb_array_pop_back(&arr);
  REQUIRE(kb_array_count(&arr) == 3);

  kb_array_pop_back(&arr);
  REQUIRE(kb_array_count(&arr) == 2);

  kb_array_pop_back(&arr);
  REQUIRE(kb_array_count(&arr) == 1);

  kb_array_pop_back(&arr);
  REQUIRE(kb_array_count(&arr) == 0);

  REQUIRE(kb_array_capacity(&arr) >= 4);
}
