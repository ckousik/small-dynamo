#ifndef ROUTING_TREE_H__
#define ROUTING_TREE_H__

#include "contact.h"
#include "key.h"

#include <utility>
#include <vector>

namespace routing {
/* _tree_node defines a node in a routing tree.
 */
struct _tree_node {
  std::pair<Key, Contact> p;
  _tree_node *left, *right, *parent;
  bool colour;

  static const bool BLACK = false;
  static const bool RED = true;

  _tree_node(const Key&, const Contact&, _tree_node*, _tree_node*, _tree_node*);
  Key key() const;
};

bool _is_black(_tree_node*);
bool _is_red(_tree_node*);
_tree_node* _parent(_tree_node*);
_tree_node* _sibling(_tree_node*);
_tree_node* _uncle(_tree_node*);
void _set_colour(_tree_node*, bool);
void _set_parent(_tree_node*, _tree_node*);

class _tree;

class _tree_iter {
 protected:
  _tree_node* cur;
  _tree_iter(_tree_node*);
  friend _tree;

 public:
  void operator--();
  void operator++();
  bool operator==(const _tree_iter&) const;
  bool operator!=(const _tree_iter&) const;
  std::pair<Key, Contact>& operator*();
  std::pair<Key, Contact>* operator->();
};

class _circ_tree_iter : public _tree_iter {
  _tree* t;
  friend _tree;
  _circ_tree_iter(_tree*, _tree_node*);

 public:
  void operator--();
  void operator++();
};

/* _tree defines a simple binary search tree used for storing
 * routing information.
 */
class _tree {
  _tree_node *root, *start_node, *end_node;

  typedef _tree_iter iterator;
  typedef _circ_tree_iter circ_iterator;
  std::size_t _sz;

  void __left_rotate(_tree_node*);
  void __right_rotate(_tree_node*);

  void __erase_fix(_tree_node*);
  _tree_node* __insert(_tree_node*);
  _tree_node* __erase(_tree_node*);

 public:
  _tree();
  iterator begin() const;
  iterator end() const;

  circ_iterator circ_begin() const;
  circ_iterator circ_end() const;

  iterator find(const Key&) const;
  iterator floor(const Key&) const;
  iterator ceil(const Key&) const;

  std::size_t size() const;

  // modifiers
  iterator erase(const Key&);
  iterator insert(const Key&, const Contact&);
  ~_tree();
};
}
#endif /* ifndef ROUTING_TREE_H__ */
