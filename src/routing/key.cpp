#include "key.h"

namespace routing {
Key::Key() { memset(this->id, 0, LENGTH * sizeof(unsigned char)); }

Key::Key(const Key& k) { memcpy(id, k.id, LENGTH * sizeof(unsigned char)); }

Key Key::random() {
  std::random_device rd;
  Key k;
  for (unsigned int i = 0; i < LENGTH; i++) {
    k.id[i] = rd();  // will be mod 256(2^8)
  }
  return k;
}

bool Key::operator<(const Key& _rhs) const {
  for (int i = LENGTH - 1; i >= 0; i--) {
    if (id[i] > _rhs.id[i]) {
      return false;
    }
    if (id[i] < _rhs.id[i]) {
      return true;
    }
  }
  return false;
}

bool Key::operator<=(const Key& _rhs) const {
  for (int i = LENGTH - 1; i >= 0; i--) {
    if (id[i] > _rhs.id[i]) {
      return false;
    }
    if (id[i] < _rhs.id[i]) {
      return true;
    }
  }
  return true;
}

bool Key::operator==(const Key& _rhs) const {
  for (int i = LENGTH - 1; i >= 0; i--) {
    if (id[i] != _rhs.id[i]) {
      return false;
    }
  }
  return true;
}

bool Key::operator>=(const Key& _rhs) const { return !(*this < _rhs); }

bool Key::operator>(const Key& _rhs) const { return !(*this <= _rhs); }

bool Key::operator!=(const Key& _rhs) const { return !(*this == _rhs); }
}
