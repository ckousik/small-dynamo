#include "tree.h"

namespace dynamo {
namespace routing {
_tree_iter::_tree_iter(_tree_node* node) { cur = node; }

void _tree_iter::operator++() {
  if (cur == nullptr) {
    return;
  }
  if (cur->right != nullptr) {
    cur = cur->right;
    while (cur->left != nullptr) {
      cur = cur->left;
    }
    return;
  }

  auto par = cur->parent;
  while (par != nullptr && cur == cur->parent->right) {
    cur = par;
    par = cur->parent;
  }
  cur = cur->parent;
}

void _tree_iter::operator--() {
  if (cur == nullptr) {
    return;
  }
  if (cur->left != nullptr) {
    cur = cur->left;
    while (cur->right != nullptr) {
      cur = cur->right;
    }
    return;
  }

  auto par = cur->parent;
  while (par != nullptr && cur == cur->parent->left) {
    cur = par;
    par = cur->parent;
  }
  cur = cur->parent;
}

bool _tree_iter::operator==(const _tree_iter& it) const {
  return cur == it.cur;
}

bool _tree_iter::operator!=(const _tree_iter& it) const {
  return cur != it.cur;
}

std::pair<Key, Contact>& _tree_iter::operator*() { return cur->p; }

std::pair<Key, Contact>* _tree_iter::operator->() { return &(cur->p); }
}
}
