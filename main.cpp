#include <bits/stdc++.h>

struct Literal {
  int idx;
  char state;
  bool operator<(const Literal &b) const { return idx < b.idx; }
};
struct AndClause {
  bool isZero = false;
  std::set<Literal> andLiterals;
  void mulClause(AndClause &b) {
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
struct SOP {
  std::vector<AndClause> sumOfClause;
  void addCaluse(AndClause &b) { sumOfClause.push_back(b); }
  void addCaluse(SOP &b) {
    sumOfClause.insert(sumOfClause.end(), b.sumOfClause.begin(),
                       b.sumOfClause.end());
  }
  void mulClause(AndClause &b) {
    for (auto &clause : sumOfClause) {
      clause.mulClause(b);
    }
  }
  void mulClause(SOP &b) {
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

SOP getBlockConstraint(Step &step) {
  SOP res;
  for (auto v : valid_list) {
    res.addCaluse(numberToAndClause(v, step.startID));
  }
  return res;
}

SOP getTransConstraint(Step &step_a, Step &step_b) {
  SOP res;
  int dx = {};
  int dy = {};
  for (auto v : valid_list) {
    int xx = valid_list % n;
    int yy = valid_list / n;
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
    step[i].startID = i * Length;
    step[i].endID = (i + 1) * Length - 1;
  }
  SOP res_1 = getBlockConstraint(step_list[0]);
  for (int i = 1; i < k; i++) {
    res_1.mulClause(getBlockConstraint(step_list[i]));
  }
  SOP res_2 = getBlockConstraint(step_list[0], step_list[1]);
  for (int i = 1; i < k - 1; i++) {
    res_2.mulClause(getTransConstraint(step_list[i]));
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
  L = 1;
  while (L < n) {
    L <<= 1;
  }
  solve(n);
  return 0;
}