#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <set>
#include <vector>
#include "tree.h"

TEST_CASE("insert and ensure size and order", "[tree, tree_iter]") {
  using namespace routing;
  auto tree = _tree();
  std::set<Key> s;
  for (int i = 0; i < 2000; i++) {
    auto key = Key::random();
    s.insert(key);
    tree.insert(key, Contact());
  }
  REQUIRE(tree.size() == s.size());
  auto prev = Key();
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    auto key = it->first;
    REQUIRE(prev < key);
    prev = key;
  }
}

TEST_CASE("erase and ensure size and order", "[tree, tree_iter]") {
  using namespace routing;
  auto tree = _tree();
  std::set<Key> s;
  for (int i = 0; i < 2000; i++) {
    auto key = Key::random();
    s.insert(key);
    tree.insert(key, Contact());
  }

  REQUIRE(tree.size() == s.size());
  std::size_t count = 0;
  std::set<Key> deleted;
  for (auto del_key : s) {
    REQUIRE(tree.find(del_key) != tree.end());
    tree.erase(del_key);
    count++;
    REQUIRE(tree.find(del_key) == tree.end());
    REQUIRE(tree.size() == s.size() - count);
    deleted.insert(del_key);
    Key prev;
    std::size_t _c = 0;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
      auto key = it->first;
      _c++;
      REQUIRE(deleted.find(key) == deleted.end());
      REQUIRE(prev < key);
      prev = key;
    }
    REQUIRE(_c == tree.size());
  }
}

TEST_CASE("check floor returns correct values") {
  using namespace routing;
  auto tree = _tree();
  for (int i = 0; i < 20; i++) {
    auto key = Key::random();
    tree.insert(key, Contact());
  }

  for (int i = 0; i < 100; i++) {
    // call floor and verify
    auto target_key = Key::random();
    auto it = tree.floor(target_key);
    if (it->first < target_key) {
      // check if next key is larger than target key or end
      ++it;
      if (it != tree.end()) {
        REQUIRE(it->first >= target_key);
      }
    } else if (it->first > target_key) {
      REQUIRE(it == tree.begin());
    }
  }
}
