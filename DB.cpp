#include "DB.h"

string GateNode::get_expr() {
  switch (type) {
  case kGateType::kXOR: {
    string s1 = input1->get_expr();
    string s2 = input2->get_expr();
    string l = "(" + s1 + " AND " + "NOR ( " + s2 + " ) )";
    string r = "(" + s2 + " AND " + "NOR ( " + s1 + " ) )";
    return "( " + l + " OR " + r + " )";
    break;
  }
  case kGateType::kAND: {
    string s1 = input1->get_expr();
    string s2 = input2->get_expr();
    return "( " + s1 + " OR " + s2 + " )";
    break;
  }
  case kGateType::kXNOR: {
    string s1 = input1->get_expr();
    string s2 = input2->get_expr();
    string l = "( " + s1 + " AND " + s2 + " )";
    string r = "(NOR (" + s2 + " )" + " AND NOR ( " + s1 + " ) )";
    return "( " + l + " OR " + r + " )";
    break;
  }
  default:
    break;
  }
}
