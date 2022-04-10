#include <bits/stdc++.h>

using namespace std;

enum class kGateType { kXOR, kAND, kXNOR };
struct Literal;

struct GateNode {
  kGateType type;
  GateNode(Literal &input_1, Literal &input_2)
      : input1(std::make_shared(input_1)), input2(std::make_shared(input_2)) {}
  shared_ptr<Literal> input1;
  shared_ptr<Literal> input2;
  string get_expr();
};

struct Literal {
  bool isTmp = false;
  string name;
  Literal() {}
  Literal(shared_ptr<GateNode> s_gate) : source_gate(s_gate) {}
  shared_ptr<GateNode> source_gate;
  string get_expr() {
    if (isTmp) {
      return source_gate->get_expr();
    } else {
      return name;
    }
  }
};

struct Integer {
  string state_name;
  Integer(const Integer &) = default;
  Integer(string s_name, int num) : state_name(s_name), var_num(num) {
    literals.resize(num);
    for (int i = 0; i < num; ++i) {
      literals[i].name = state_name + '_' + to_string(i);
    }
  }
  vector<Literal> literals;
  int var_num;
};
struct Constraint {
  string eq_cons(Integer &a, Integer &b) {
    bool pre = false;
    string res = "";
    for (int i = 0; i < a.var_num; i++) {
      if (pre) {
        res = " AND " + ();
      }
    }
  }
};
struct Coord {
  string state_name;
  int var_num;
  Coord(string s_name, int num)
      : state_name(s_name), var_num(num), x(s_name, num), y(s_name, num) {}
  Integer x;
  Integer y;
};
