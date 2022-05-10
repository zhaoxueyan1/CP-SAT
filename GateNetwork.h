/**
 * @file GateNetwork.h
 * @author Xueyan Zhao (zhaoxueyan131@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-10 03:41:47
 * @copyright Copyright (c) 2021-2022 PCNL EDA.
 */

#include <iostream>
#include <set>
#include <vector>
namespace GN {

struct Literal {
  int idx;
  char state;
  bool operator<(const Literal &b) const { return idx < b.idx; }
};

struct AndClause {
  bool isZero = false;
  std::set<Literal> andLiterals;
  void mulClause(const AndClause &b) {
    if (isZero || b.isZero) {
      isZero = true;
    }
    for (auto &l : b.andLiterals) {
      if (andLiterals.count(l)) {
        auto itr = andLiterals.find(l);
        if (l.state != itr->state) {
          isZero = true;
          break;
        }
      } else {
        andLiterals.insert(l);
      }
    }
  }
  // void addCaluse(AndClause &b) {}
};

struct OrClause {
  bool isOne = false;
  std::set<Literal> orLiterals;
  void addClause(const OrClause &b) {
    if (isOne || b.isOne) {
      isOne = true;
      return;
    }
    for (auto &l : b.orLiterals) {
      if (orLiterals.count(l)) {
        auto itr = orLiterals.find(l);
        if (l.state != itr->state) {
          isOne = true;
          break;
        }
      } else {
        orLiterals.insert(l);
      }
    }
  }
  // void addCaluse(AndClause &b) {}
};

struct CNF {
  std::vector<OrClause> mulOfClause;
  void mulClause(const OrClause &b) { mulOfClause.push_back(b); }
  void mulClause(const CNF &b) {
    mulOfClause.insert(mulOfClause.end(), b.mulOfClause.begin(),
                       b.mulOfClause.end());
  }
  void addClause(const OrClause &b) {
    if (mulOfClause.empty()) {
      mulOfClause.push_back(b);
    }
    for (auto &clause : mulOfClause) {
      clause.addClause(b);
    }
  }
  void addClause(const CNF &b) {
    if (mulOfClause.empty()) {
      mulOfClause = b.mulOfClause;
    }
    std::vector<OrClause> res;
    for (auto &clause : b.mulOfClause) {
      CNF t;
      t.mulOfClause = this->mulOfClause;
      t.addClause(clause);
      res.insert(res.end(), t.mulOfClause.begin(), t.mulOfClause.end());
    }
    mulOfClause = res;
  }
};

struct SOP {
  std::vector<AndClause> sumOfClause;
  void addCaluse(const AndClause &b) { sumOfClause.push_back(b); }
  void addCaluse(const SOP &b) {
    sumOfClause.insert(sumOfClause.end(), b.sumOfClause.begin(),
                       b.sumOfClause.end());
  }
  void mulClause(const AndClause &b) {
    for (auto &clause : sumOfClause) {
      clause.mulClause(b);
    }
  }
  void mulClause(const SOP &b) {
    std::vector<AndClause> res;
    for (auto &clause : b.sumOfClause) {
      SOP t;
      t.sumOfClause = this->sumOfClause;
      t.mulClause(clause);
      res.insert(res.end(), t.sumOfClause.begin(), t.sumOfClause.end());
    }
    sumOfClause = res;
  }
};

struct Step {
  int startID;
  int endID;
};

struct Literal {
  int idx;
  char state;
  bool operator<(const Literal &b) const { return idx < b.idx; }
};

enum class kGateType { kOR, kAND, kXNOR };

struct GateNode {
  kGateType type;
  bool isLiteral = false;
  Literal l;
};

struct Node {
  int idx;
  int fa;
  GateNode gate;
  std::vector<int> sons;
};

struct Graph {
  int v;
  int root;
  int maxLiteralID;
  std::vector<Node> nodes;

  int addSonGate(GateNode g, int fa) {
    Node t;
    t.fa = fa;
    t.idx = v++;
    t.gate = g;
    if (fa != t.idx) {
      nodes[t.fa].sons.push_back(t.idx);
    }
    return v - 1;
  }
  void addInputLiterals(std::vector<Literal> literals, int fa) {
    for (auto &l : literals) {
      /* code */
      GateNode t;
      t.isLiteral = true;
      t.l = l;
      addSonGate(t, fa);
    }
  }
  CNF tseitinTransform(int idx) {
    CNF res;
    GateNode n_v = nodes[idx];
    if (n_v.isLiteral) {
      res.addClause(c);
      return v;
    }
    switch (n_v.type) {
    case /* constant-expression */:
      /* code */
      break;

    default:
      break;
    }
  }
  // void addSonGate(GateNode g, int fa) {}
};

} // namespace GN