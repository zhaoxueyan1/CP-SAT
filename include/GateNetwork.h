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
#pragma once

namespace GN {

struct Literal {
  int idx;
  bool state = true;
  Literal getNeg() {
    Literal res = *this;
    res.state = !state;
    return res;
  }
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
  OrClause() {}
  OrClause(const OrClause &b) {
    orLiterals = b.orLiterals;
    isOne = b.isOne;
  }
  std::set<Literal> orLiterals;
  void addLiteral(const Literal &b) {
    if (isOne) {
      isOne = true;
      return;
    }
    if (orLiterals.count(b)) {
      auto itr = orLiterals.find(b);
      if (b.state != itr->state) {
        isOne = true;
      }
    } else {
      orLiterals.insert(b);
    }
  }
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
  int idx;
  int startID;
  int endID;
};

enum class kGateType { kOR, kAND, kXNOR };

struct GateNode {
  GateNode() = default;
  GateNode(kGateType g_type) : type(g_type) {}
  GateNode(Literal l) : literal(l), isLiteral(true) {}
  kGateType type;
  bool isLiteral = false;
  Literal literal;
};

struct Node {
  int idx;
  int fa;
  GateNode gate;
  std::vector<int> sons;
};

struct GateNetwork {
  int v;
  int root;
  int maxLiteralID = 0;
  std::vector<Node> nodes;
  void init(int max_literal_id) {
    nodes.clear();
    root = v = 0;
    maxLiteralID = max_literal_id;
  }
  int addSonGate(GateNode g, int fa) {
    Node t;
    t.fa = fa;
    t.idx = v++;
    t.gate = g;
    if (g.isLiteral) {
      maxLiteralID = std::max(maxLiteralID, g.literal.idx);
    }
    if (fa != t.idx) {
      nodes[t.fa].sons.push_back(t.idx);
    }
    nodes.push_back(t);
    return v - 1;
  }
  void addInputLiterals(std::vector<Literal> literals, int fa) {
    for (auto &l : literals) {
      /* code */
      // maxLiteralID = std::max(maxLiteralID, l.idx);
      GateNode t;
      t.isLiteral = true;
      t.literal = l;
      addSonGate(t, fa);
    }
  }

  CNF tseitinTransform(int idx, Literal pre_literal,
                       bool need_tseitin = false) {
    CNF res;
    Node &n_v = nodes[idx];
    if (n_v.gate.isLiteral) {
      // res.addClause(c);
      return res;
    }
    if (need_tseitin == false && n_v.gate.type != kGateType::kAND) {
      Literal n_l;
      n_l.idx = ++maxLiteralID;
      need_tseitin = true;
      pre_literal = n_l;
    }

    std::vector<Literal> son_literals;
    for (auto son_id : n_v.sons) {
      Node &son_v = nodes[son_id];
      if (son_v.gate.isLiteral) {
        son_literals.push_back(son_v.gate.literal);
      } else {
        Literal n_l;
        n_l.idx = ++maxLiteralID;
        res.mulClause(tseitinTransform(son_id, n_l, true));
        son_literals.push_back(n_l);
      }
    }
    switch (n_v.gate.type) {
    case kGateType::kOR: {
      // right clause
      Literal neg_pre_literal = pre_literal.getNeg();
      OrClause c_l;
      c_l.addLiteral(neg_pre_literal);
      for (auto son_literal : son_literals) {
        c_l.addLiteral(son_literal);

        Literal neg_son = son_literal.getNeg();
        OrClause c_r_i;
        c_r_i.addLiteral(pre_literal);
        c_r_i.addLiteral(neg_son);
        res.mulClause(c_r_i);
      }
      res.mulClause(c_l);
      return res;
    }
    case kGateType::kAND: {

      // don't need tseitin transformation
      if (!need_tseitin) {
        for (auto son_literal : son_literals) {
          OrClause c_l_i;
          c_l_i.addLiteral(son_literal);
          res.mulClause(c_l_i);
        }
        return res;
      }

      // need transformation
      Literal neg_pre_literal = pre_literal.getNeg();
      // right clause
      OrClause c_r;
      c_r.addLiteral(pre_literal);

      // left clause
      for (auto son_literal : son_literals) {
        Literal neg_son = son_literal.getNeg();
        // neg_son.state = !son_literal.state;
        c_r.addLiteral(neg_son);
        OrClause c_l_i;
        c_l_i.addLiteral(neg_pre_literal);
        c_l_i.addLiteral(son_literal);
        res.mulClause(c_l_i);
      }
      res.mulClause(c_r);
      return res;
    }
    case kGateType::kXNOR: {
      break;
    }

    default:
      break;
    }
  }
  // void addSonGate(GateNode g, int fa) {}
};

} // namespace GN