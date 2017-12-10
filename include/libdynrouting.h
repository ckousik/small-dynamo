#ifndef LIBDYNROUTING_H__
#define LIBDYNROUTING_H__

#include <vector>

namespace dynamo{
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
  virtual ~RoutingTableBase() {};
  virtual void insert(const Key&, const Contact&) = 0;
  virtual void erase(const Key&) = 0;
  virtual std::pair<Contact, bool> get(const Key&) const = 0;
  virtual std::vector<Contact> get_preferred(const Key&, 
      std::size_t) const = 0;
};

RoutingTableBase* get_routing_table();
}
}
#endif
