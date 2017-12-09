#ifndef ROUTING_KEY_H__
#define ROUTING_KEY_H__

#include <cstring>
#include <random>

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
}
#endif /* ifndef ROUTING_KEY_H__*/
