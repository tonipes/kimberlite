#include <catch.hpp>

#include <kb/table.h>

TEST_CASE("zero initialized table should be valid and empty", "[table]") {
  kb_table table {};
  
  REQUIRE(kb_table_count      (&table)            == 0);
  REQUIRE(kb_table_insert     (&table, 123, 123)  == false);
  REQUIRE(kb_table_get        (&table, 123)       == UINT32_MAX);
  REQUIRE(kb_table_get_hash   (&table, 123)       == UINT32_MAX);
  REQUIRE(kb_table_find_index (&table, 123)       == UINT32_MAX);
  REQUIRE(kb_table_has        (&table, 123)       == false);
  REQUIRE(kb_table_remove     (&table, 123)       == false);
}

TEST_CASE("initialized table should have correct capacity and count", "[table]") {
  kb_table table {};
  
  kb_table_create(&table, 10);

  REQUIRE(kb_table_count(&table)  == 0);
  REQUIRE(table.count             == 0);
  REQUIRE(table.capacity          == 10);
}

TEST_CASE("table_get and table_get_hash should work after insert", "[table]") {
  kb_table table {};
  
  kb_table_create(&table, 10);

  REQUIRE(kb_table_insert   (&table, 123, 456));
  REQUIRE(kb_table_get      (&table, 123) == 456);
  REQUIRE(kb_table_get_hash (&table, 456) == 123);
}

TEST_CASE("table insert should increase count", "[table]") {
  kb_table table {};
  
  kb_table_create(&table, 10);

  REQUIRE(kb_table_count(&table) == 0);
  REQUIRE(kb_table_insert   (&table, 123, 456));
  REQUIRE(kb_table_count(&table) == 1);
  REQUIRE(kb_table_insert   (&table, 223, 356));
  REQUIRE(kb_table_count(&table) == 2);
}

TEST_CASE("table remove should decreate count", "[table]") {
  kb_table table {};
  
  kb_table_create(&table, 10);

  REQUIRE(kb_table_insert   (&table, 123, 156));
  REQUIRE(kb_table_insert   (&table, 223, 256));
  REQUIRE(kb_table_insert   (&table, 323, 356));
  REQUIRE(kb_table_count    (&table) == 3);

  REQUIRE(kb_table_remove   (&table, 123));
  REQUIRE(kb_table_count    (&table) == 2);

  REQUIRE(kb_table_remove   (&table, 223));
  REQUIRE(kb_table_count    (&table) == 1);

  REQUIRE(kb_table_remove   (&table, 323));
  REQUIRE(kb_table_count    (&table) == 0);
}

TEST_CASE("removed should not be available", "[table]") {
  kb_table table {};
  
  kb_table_create(&table, 10);

  REQUIRE(kb_table_insert   (&table, 123, 156));
  REQUIRE(kb_table_insert   (&table, 223, 256));
  REQUIRE(kb_table_insert   (&table, 323, 356));

  REQUIRE(kb_table_remove   (&table, 123));
  REQUIRE(kb_table_get      (&table, 123) == UINT32_MAX);

  REQUIRE(kb_table_remove   (&table, 223));
  REQUIRE(kb_table_get      (&table, 223) == UINT32_MAX);

  REQUIRE(kb_table_remove   (&table, 323));
  REQUIRE(kb_table_get      (&table, 323) == UINT32_MAX);
}


TEST_CASE("inserting existing should not replace", "[table]") {
  kb_table table {};
  
  kb_table_create(&table, 10);

  REQUIRE(kb_table_insert   (&table, 123, 456));
  REQUIRE(kb_table_get      (&table, 123) == 456);

  REQUIRE(kb_table_insert   (&table, 123, 555) == false);
  REQUIRE(kb_table_get      (&table, 123) == 456);
  
  REQUIRE(kb_table_count(&table) == 1);
}

TEST_CASE("copy should work", "[table]") {
  kb_table table {};
  kb_table table2 {};
  
  kb_table_create(&table, 10);

  REQUIRE(kb_table_insert   (&table, 123, 156));
  REQUIRE(kb_table_insert   (&table, 223, 256));
  REQUIRE(kb_table_insert   (&table, 323, 356));
  
  REQUIRE(kb_table_count    (&table) == 3);

  kb_table_copy(&table2, &table);

  REQUIRE(kb_table_get   (&table2, 123) == 156);
  REQUIRE(kb_table_get   (&table2, 223) == 256);
  REQUIRE(kb_table_get   (&table2, 323) == 356);
  
  REQUIRE(kb_table_count(&table2) == 3);
  REQUIRE(table2.capacity == 10);
}
