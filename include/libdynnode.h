#ifndef LIBDYNNODE_H__
#define LIBDYNNODE_H__

namespace dynamo {
namespace node {
class NodeBase {
  void store();
  void write();
};

NodeBase* new_node();
}
}
#endif
