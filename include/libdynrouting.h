#ifndef LIBDYNROUTING_H__
#define LIBDYNROUTING_H__

#include <vector>

namespace routing {

class Key {
 public:
  // constructors
  Key();
  Key(const Key&);

  // static generator
  static Key random();
  static const unsigned int LENGTH = 20;

  bool operator<(const Key&) const;
  bool operator<=(const Key&) const;
  bool operator==(const Key&) const;
  bool operator>=(const Key&) const;
  bool operator>(const Key&) const;
  bool operator!=(const Key&) const;

 private:
  unsigned char id[LENGTH];
};

// TODO: Populate contact fields
class Contact {};

class RoutingTableBase {
 public:
  virtual void insert(const Key&, const Contact&) = 0;
  virtual void erase(const Key&) = 0;
  virtual std::pair<bool, Contact> get(const Key&) = 0;
  virtual std::vector<Contact> get_preferred(const Key&) = 0;
};

RoutingTableBase* get_routing_table();
}
#endif
