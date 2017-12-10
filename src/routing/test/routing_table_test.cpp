#include "catch.hpp"
#include "libdynrouting.h"

#include <set>

using namespace dynamo;
TEST_CASE("insert values and find", "[tree_table]") {
  using namespace routing;
  auto table = get_routing_table();
  std::set<Key> s;
  for (int i = 0; i < 1000; i++) {
    auto key = Key::random();
    table->insert(key, Contact());
    s.insert(key);
  }
  for (auto key : s) {
    auto p = table->get(key);
    REQUIRE(p.second == true);
  }
  delete table;
}
