#include "tree.h"

namespace dynamo {
namespace routing {

Key _tree_node::key() const { return this->p.first; }

_tree_node::_tree_node(const Key& key, const Contact& contact,
                       _tree_node* par = nullptr, _tree_node* l = nullptr,
                       _tree_node* r = nullptr) {
  p = std::make_pair(key, contact);
  parent = par;
  this->left = l;
  this->right = r;
}

bool _is_black(_tree_node* node) {
  if (node == nullptr) {
    return true;
  }
  return node->colour == _tree_node::BLACK;
}

bool _is_red(_tree_node* node) { return !(_is_black(node)); }

_tree_node* _parent(_tree_node* node) {
  if (node == nullptr) {
    return nullptr;
  }
  return node->parent;
}

_tree_node* _sibling(_tree_node* node) {
  if (node == nullptr || node->parent == nullptr) {
    return nullptr;
  }
  if (node == node->parent->left) {
    return node->parent->right;
  }
  return node->parent->left;
}

_tree_node* _uncle(_tree_node* node) { return _sibling(_parent(node)); }

void _set_parent(_tree_node* node, _tree_node* par) {
  if (node == nullptr) {
    return;
  }
  node->parent = par;
}

void _set_colour(_tree_node* node, bool colour) {
  if (node == nullptr) {
    return;
  }
  node->colour = colour;
}
}
}
