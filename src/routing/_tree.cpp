#include <queue>
#include "tree.h"

#ifdef DEBUG
#include <cassert>
#endif

namespace dynamo {
namespace routing {

_tree::_tree() {
  root = nullptr;
  start_node = nullptr;
  end_node = nullptr;
  _sz = 0;
}

_tree::~_tree() {
  if (root == nullptr) {
    return;
  }
  std::queue<_tree_node*> q;
  q.push(root);
  while (!q.empty()) {
    auto x = q.front();
    q.pop();
    if (x->left != nullptr) {
      q.push(x->left);
    }
    if (x->right != nullptr) {
      q.push(x->right);
    }
    delete x;
  }
}

_tree_iter _tree::find(const Key& target_key) const {
  if (root == nullptr) {
    return this->end();
  }
  auto cur = root;
  while (cur != nullptr) {
    Key key = cur->key();
    if (key == target_key) {
      break;
    }
    if (key > target_key) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }
  return _tree_iter(cur);
}

_tree_iter _tree::begin() const { return _tree_iter(start_node); }

_tree_iter _tree::end() const { return _tree_iter(nullptr); }

_tree_node* _tree::__insert(_tree_node* node) {
  // root condition
  node->left = node->right = nullptr;
  if (root == nullptr) {
    root = node;
    start_node = root;
    end_node = root;
    _sz++;
    return root;
  }

  auto cur = root, par = root;
  auto key = node->p.first;
  while (cur != nullptr) {
    par = cur;
    if (key == cur->key()) {
      cur->p = node->p;
      delete node;
      return cur;
    }

    if (key < cur->key()) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }
  node->parent = par;

  _sz++;
  if (key < par->key()) {
    par->left = node;
    if (par == start_node) {
      start_node = par->left;
    }
  } else {
    par->right = node;
    if (par == end_node) {
      end_node = par->right;
    }
  }
  return node;
}

_tree_iter _tree::insert(const Key& key, const Contact& contact) {
#ifdef DEBUG
  assert(_is_black(root));
#endif
  auto node = new _tree_node(key, contact, 0x0, 0x0, 0x0);
  _set_colour(node, _tree_node::RED);
  node = __insert(node);
  while (node != root && _is_red(_parent(node))) {
    // case 1:
    if (_is_red(_uncle(node))) {
      _set_colour(_parent(node), _tree_node::BLACK);
      _set_colour(_uncle(node), _tree_node::BLACK);
      _set_colour(_parent(_parent(node)), _tree_node::RED);
      node = _parent(_parent(node));
      continue;
    }
    // Case 2:
    // 1. Check if left child of right child
    // 2. Check if right child of left child
    if (node == node->parent->left &&
        node->parent == node->parent->parent->right) {
      node = node->parent;
      __right_rotate(node);
    } else if (node == node->parent->right &&
               node->parent == node->parent->parent->left) {
      node = node->parent;
      __left_rotate(node);
    }
    _set_colour(_parent(node), _tree_node::BLACK);
    _set_colour(_parent(_parent(node)), _tree_node::RED);
    if (node == node->parent->left) {
      __right_rotate(_parent(_parent(node)));
    } else {
      __left_rotate(_parent(_parent(node)));
    }
  }
  _set_colour(root, _tree_node::BLACK);
#ifdef DEBUG
  assert(node != 0x0);
  if (_is_red(node)) {
    assert(_is_black(node->left));
    assert(_is_black(node->right));
  }
#endif
  return _tree_iter(node);
}

_tree_iter _tree::erase(const Key& key) {
  auto node = __erase(find(key).cur);
  if (node != nullptr) {
    __erase_fix(node);
  }
  return _tree_iter(node);
}

_tree_node* _tree::__erase(_tree_node* node) {
  while (node != nullptr) {
    if (node->left == nullptr && node->right == nullptr) {
      auto par = node->parent;
      if (node == root) {
        root = nullptr;
      } else {
        if (node == par->left) {
          par->left = nullptr;
        } else {
          par->right = nullptr;
        }
      }

      if (node == start_node) {
        start_node = node->parent;
      }
      if (node == end_node) {
        end_node = node->parent;
      }

      delete node;
      _sz--;
      return par;
    }
    if (node->left != nullptr && node->right == nullptr) {
      auto next = node->left;
      next->parent = node->parent;
      if (node == root) {
        root = next;
      } else {
        if (node == node->parent->left) {
          node->parent->left = next;
        } else {
          node->parent->right = next;
        }
      }

      if (node == end_node) {
        end_node = next;
        while (end_node->right != nullptr) {
          end_node = end_node->right;
        }
      }

      _sz--;
      delete node;
      return next;

    } else if (node->left == nullptr && node->right != nullptr) {
      auto next = node->right;
      next->parent = node->parent;
      if (node == root) {
        root = next;
      } else {
        if (node == node->parent->left) {
          node->parent->left = next;
        } else {
          node->parent->right = next;
        }
      }

      if (node == start_node) {
        start_node = next;
        while (start_node->left != nullptr) {
          start_node = start_node->left;
        }
      }
      _sz--;
      delete node;
      return next;
    }
    // 2 children
    auto pred = node->left;
    while (pred->right != nullptr) {
      pred = pred->right;
    }
    node->p = pred->p;
    node = pred;
  }
  return nullptr;
}

void _tree::__erase_fix(_tree_node* node) {
  while (node != root && _is_black(node)) {
    if (node == node->parent->left) {
      auto w = node->parent->right;
      if (_is_red(w)) {
        _set_colour(w, _tree_node::BLACK);
        _set_colour(_parent(node), _tree_node::RED);
        __left_rotate(_parent(node));
        w = node->parent->right;
      }
      if (w != nullptr && _is_black(w->left) && _is_black(w->right)) {
        _set_colour(w, _tree_node::RED);
        node = _parent(node);
      } else {
        if (w != nullptr && _is_black(w->right)) {
          _set_colour(w->left, _tree_node::RED);
          _set_colour(w, _tree_node::RED);
          __right_rotate(w);
          w = node->parent->right;
        }
        _set_colour(w, _is_red(_parent(node)));
        _set_colour(_parent(node), _tree_node::BLACK);
        if (w != nullptr) {
          _set_colour(w->right, _tree_node::BLACK);
        }
        __left_rotate(_parent(node));
        node = root;
      }
    } else {
      auto w = node->parent->left;
      if (_is_red(w)) {
        _set_colour(w, _tree_node::BLACK);
        _set_colour(_parent(node), _tree_node::RED);
        __right_rotate(_parent(node));
        w = node->parent->left;
      }
      if (w != nullptr && _is_black(w->left) && _is_black(w->right)) {
        _set_colour(w, _tree_node::RED);
        node = _parent(node);
      } else {
        if (w != nullptr && _is_black(w->left)) {
          _set_colour(w->right, _tree_node::RED);
          _set_colour(w, _tree_node::RED);
          __left_rotate(w);
          w = node->parent->left;
        }
        _set_colour(w, _is_red(_parent(node)));
        _set_colour(_parent(node), _tree_node::BLACK);
        if (w != nullptr) {
          _set_colour(w->left, _tree_node::BLACK);
        }
        __right_rotate(_parent(node));
        node = root;
      }
    }
  }
  _set_colour(root, _tree_node::BLACK);
}

_tree_iter _tree::floor(const Key& target_key) const {
  auto closest = root, cur = root;
  if (start_node != nullptr && start_node->key() > target_key) {
    return _tree_iter(start_node);
  }
  while (cur != nullptr) {
    auto key = cur->key();
    if (key == target_key) {
      closest = cur;
      break;
    }
    if (target_key > key) {
      closest = cur;
      cur = cur->right;
    } else {
      cur = cur->left;
    }
  }
  // smallest value in the tree is larger than
  // the requested key.
  return _tree_iter(closest);
}

_tree_iter _tree::ceil(const Key& target_key) const {
  auto closest = root, cur = root;
  if (end_node != nullptr && end_node->key() < target_key) {
    return _tree_iter(end_node);
  }
  while (cur != nullptr) {
    auto key = cur->key();
    if (key == target_key) {
      closest = cur;
      break;
    }
    if (target_key < key) {
      closest = cur;
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }
  return _tree_iter(closest);
}

std::size_t _tree::size() const { return _sz; }

// left rotate and right rotate
//    y                    x
//   / \                  / \
//  x  C  ---- RR--->    A   y
// / \    <--- LR----       / \
// A  B                    B   C

void _tree::__left_rotate(_tree_node* x) {
  if (x == nullptr || x->right == nullptr) {
    return;
  }
  auto y = x->right;
  x->right = y->left;       // X to B
  _set_parent(y->left, x);  // B to X
  _set_parent(y, x->parent);
  if (x->parent != nullptr) {
    if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
  } else {
    root = y;
  }
  x->parent = y;
  y->left = x;
}

void _tree::__right_rotate(_tree_node* y) {
  if (y == nullptr || y->left == nullptr) {
    return;
  }
  auto x = y->left;
  y->left = x->right;
  _set_parent(x->right, y);
  _set_parent(x, y->parent);
  if (y->parent != 0x0) {
    if (y == y->parent->left) {
      y->parent->left = x;
    } else {
      y->parent->right = x;
    }
  } else {
    root = x;
  }
  y->parent = x;
  x->right = y;
}
}
}
