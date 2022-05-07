#include <bits/stdc++.h>

using namespace std;

enum class kState { kPositive, kNegetive, kZero };

struct Integer {
  string state_name;
  Integer(const Integer &) = default;
  Integer(string s_name, int num) : state_name(s_name), var_num(num) {
    literals.resize(num);
    for (int i = 0; i < num; ++i) {
      literals[i].name = state_name + '_' + to_string(i);
    }
  }
  Integer getSelfAddOne() {
    Integer res(*this);
    Literal t;
    t.name = "true";
    for (int i = 0; i < var_num; i++) {
      GateNode g_xor(&t, &res.literals[i], kGateType::kXOR);
      GateNode g_and(&t, &res.literals[i], kGateType::kAND);
      res.literals[i].name = g_xor.get_expr();
      t.name = g_and.get_expr();
    }
    return res;
  }
  vector<Literal> literals;
  int var_num;
};
struct Constraint {
  string eq_cons(Integer &a, Integer &b) {
    bool pre = false;
    string res = "";
    for (int i = 0; i < a.var_num; i++) {
      GateNode g = GateNode(&a.literals[i], &b.literals[i], kGateType::kXNOR);
      string expr = g.get_expr();
      if (!pre) {
        res += expr;
        pre = true;

      } else {
        res += " AND " + expr;
      }
    }
    return res;
  }

  string neq_cons(Integer &a, Integer &b) {
    string res = eq_cons(a, b);
    res = "NOR" + res;
    return res;
  }

  string le_cons(Integer &a, Integer &b) {
    bool pre = false;
    string res = "";
    string pre_cons = "";
    for (int i = 0; i < a.var_num; i++) {
      GateNode g = GateNode(&a.literals[i], &b.literals[i], kGateType::kXNOR);
      string expr = g.get_expr();
      if (!pre) {
        res += expr;
        pre = true;

      } else {
        res += " AND " + expr;
      }
    }
    return res;
  }

  vector<string> all_clauses;
};
struct Coord {
  string state_name;
  int var_num;
  Coord(string s_name, int num)
      : state_name(s_name), var_num(num), x(s_name, num), y(s_name, num) {}
  Integer x;
  Integer y;
};
