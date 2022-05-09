#include <bits/stdc++.h>

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
  void addClause(OrClause &b) {
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
  void mulCaluse(OrClause &b) { mulOfClause.push_back(b); }
  void mulCaluse(CNF &b) {
    mulOfClause.insert(mulOfClause.end(), b.mulOfClause.begin(),
                       b.mulOfClause.end());
  }
  void addClause(OrClause &b) {
    for (auto &clause : mulOfClause) {
      clause.addClause(b);
    }
  }
  void addClause(CNF &b) {
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

bool block_map[100][100];
std::set<int> valid_list;
int Length;
int n;
int k;

AndClause numberToAndClause(int number, int literal_start) {
  AndClause res;
  for (int i = 0; i < Length; i++) {
    Literal l;
    l.idx = literal_start + i;
    l.state = (1 << i) & number ? 1 : 0;
    res.andLiterals.insert(l);
  }
  return res;
}

SOP getBlockConstraint(const Step &step) {
  SOP res;
  for (auto v : valid_list) {
    res.addCaluse(numberToAndClause(v, step.startID));
  }
  return res;
}

SOP getTransConstraint(const Step &step_a, const Step &step_b) {
  SOP res;
  int dx[] = {0, 1, 0, -1};
  int dy[] = {1, 0, -1, 0};
  for (auto v : valid_list) {
    int xx = v % n;
    int yy = v / n;
    for (int i = 0; i < 4; i++) {
      int n_xx = xx + dx[i];
      int n_yy = yy + dy[i];
      int n_v = n_yy * n + n_xx;
      if (0 <= n_xx && n_xx < n && 0 <= n_yy && n_yy < n &&
          valid_list.count(n_v)) {
        AndClause a = numberToAndClause(v, step_a.startID);
        AndClause b = numberToAndClause(n_v, step_b.startID);
        a.mulClause(b);
        res.addCaluse(a);
      }
    }
  }
  return res;
}

SOP getKthSop(int k) {
  std::vector<Step> step_list(k);
  for (int i = 0; i < k; i++) {
    step_list[i].startID = i * Length;
    step_list[i].endID = (i + 1) * Length - 1;
  }
  SOP res_1 = getBlockConstraint(step_list[0]);
  for (int i = 1; i < k; i++) {
    res_1.mulClause(getBlockConstraint(step_list[i]));
  }
  SOP res_2 = getTransConstraint(step_list[0], step_list[1]);
  for (int i = 1; i < k - 1; i++) {
    res_2.mulClause(getTransConstraint(step_list[i], step_list[i + 1]));
  }
  res_1.mulClause(res_2);
}

SOP solve(int n) {
  k = 2 * (n - 1);
  while (k < n * n) {
    SOP s = getKthSop(k);
  }
}

int main() {
  std::string file_name = "./bench/1.in";
  freopen(file_name.c_str(), "r", stdin);
  scanf("%d", &n);
  getchar();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      block_map[i][j] = getchar();
    }
    getchar();
  }
  Length = 1;
  while (Length < n) {
    Length <<= 1;
  }
  solve(n);
  return 0;
}