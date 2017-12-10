#include "libdynrouting.h"

#include "tree.h"

namespace dynamo {
namespace routing {

class TreeTable : public RoutingTableBase {
  _tree _t;

 public:
  void insert(const Key&, const Contact&);
  void erase(const Key&);
  std::vector<Contact> get_preferred(const Key&, std::size_t) const;
  std::pair<Contact, bool> get(const Key&) const;
  TreeTable() {}
};

inline void TreeTable::insert(const Key& key, const Contact& contact) {
  _t.insert(key, contact);
}

inline void TreeTable::erase(const Key& key) { _t.erase(key); }

std::vector<Contact> TreeTable::get_preferred(const Key& key,
                                              std::size_t max_count) const {
  std::vector<Contact> result;
  auto it = _t.floor(key);
  auto start = it;
  std::size_t count = 0;
  for (; it != _t.end() && count < max_count; ++it) {
    count++;
    result.push_back(it->second);
  }
  for (it = _t.begin(); it != start && it != _t.end() && count < max_count;
       ++it) {
    count++;
    result.push_back(it->second);
  }
  return result;
}

std::pair<Contact, bool> TreeTable::get(const Key& key) const {
  auto it = _t.find(key);
  if (it != _t.end()) {
    return std::make_pair(it->second, true);
  }
  return std::make_pair(Contact(), false);
}

RoutingTableBase* get_routing_table() {
  auto table = new TreeTable();
  return table;
}
}
}
