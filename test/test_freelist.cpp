#include <catch.hpp>

#include <kb/freelist.h>

TEST_CASE("zero initialized freelist should be empty and not freak out", "[freelist]") {
  kb_freelist freelist {};
  
  REQUIRE(kb_freelist_take (&freelist)      == UINT32_MAX);
  REQUIRE(kb_freelist_count(&freelist)      == 0);
  REQUIRE(kb_freelist_get_sparse(&freelist) == nullptr);
  REQUIRE(kb_freelist_get_dense(&freelist)  == nullptr);
}

TEST_CASE("initialized freelist should have correct capacity and count", "[freelist]") {
  kb_freelist freelist {};
  
  kb_freelist_create(&freelist, 10);

  REQUIRE(kb_freelist_count(&freelist)  == 0);
  REQUIRE(freelist.count                == 0);
  REQUIRE(freelist.capacity             == 10);
}

TEST_CASE("freelist take should increase count", "[freelist]") {
  kb_freelist freelist {};
  
  kb_freelist_create(&freelist, 10);

  REQUIRE(kb_freelist_count(&freelist) == 0);
  kb_freelist_take(&freelist);
  REQUIRE(kb_freelist_count(&freelist) == 1);
  kb_freelist_take(&freelist);
  REQUIRE(kb_freelist_count(&freelist) == 2);
  kb_freelist_take(&freelist);
  REQUIRE(kb_freelist_count(&freelist) == 3);
  kb_freelist_take(&freelist);
  REQUIRE(kb_freelist_count(&freelist) == 4);
}

TEST_CASE("freelist should not allow same handle to be retuned more than once", "[freelist]") {
  kb_freelist freelist {};

  kb_freelist_create(&freelist, 10);

  uint32_t h = kb_freelist_take(&freelist);
  kb_freelist_take(&freelist);
  kb_freelist_take(&freelist);
  kb_freelist_take(&freelist);
  kb_freelist_take(&freelist);
  kb_freelist_take(&freelist);
  kb_freelist_take(&freelist);
  kb_freelist_take(&freelist);
  kb_freelist_take(&freelist);

  REQUIRE(kb_freelist_free(&freelist, h) == true);
  REQUIRE(kb_freelist_free(&freelist, h) == false);
  REQUIRE(kb_freelist_free(&freelist, h) == false);
  REQUIRE(kb_freelist_free(&freelist, h) == false);
  REQUIRE(kb_freelist_free(&freelist, h) == false);
};

TEST_CASE("freelist return should decrease count", "[freelist]") {
  kb_freelist freelist {};
  
  kb_freelist_create(&freelist, 10);

  uint32_t a1 = kb_freelist_take(&freelist);
  uint32_t a2 = kb_freelist_take(&freelist);
  uint32_t a3 = kb_freelist_take(&freelist);
  uint32_t a4 = kb_freelist_take(&freelist);

  REQUIRE(kb_freelist_count(&freelist) == 4);

  REQUIRE(kb_freelist_free(&freelist, a1));
  REQUIRE(kb_freelist_count(&freelist) == 3);

  REQUIRE(kb_freelist_free(&freelist, a2));
  REQUIRE(kb_freelist_count(&freelist) == 2);

  REQUIRE(kb_freelist_free(&freelist, a3));
  REQUIRE(kb_freelist_count(&freelist) == 1);

  REQUIRE(kb_freelist_free(&freelist, a4));
  REQUIRE(kb_freelist_count(&freelist) == 0);
}

TEST_CASE("freelist take should give indices in order", "[freelist]") {
  kb_freelist freelist {};
  
  kb_freelist_create(&freelist, 10);

  REQUIRE(kb_freelist_take(&freelist) == 0);
  REQUIRE(kb_freelist_take(&freelist) == 1);
  REQUIRE(kb_freelist_take(&freelist) == 2);
  REQUIRE(kb_freelist_take(&freelist) == 3);
  REQUIRE(kb_freelist_take(&freelist) == 4);
  REQUIRE(kb_freelist_take(&freelist) == 5);
}
