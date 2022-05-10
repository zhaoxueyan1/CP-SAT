#include "GateNetwork.h"
#include <bits/stdc++.h>

bool block_map[100][100];
std::set<int> valid_list;
int Length, n, k;

using namespace GN;
#if 0
namespace SOPSolve {
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
  // printf("%d\n", res_1.sumOfClause.size());
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
    int i = 0;
    for (auto &vec : s.sumOfClause) {
      int j = 0;
      for (auto &l : vec.andLiterals) {
        if (l.state == 0) {
          printf("not ");
        }
        printf("x%d %s", l.idx,
               (j++) == vec.andLiterals.size() - 1 ? ")" : "or ");
      }
      printf("%s", (i++) == s.sumOfClause.size() - 1 ? "\n" : "and ");
    }
  }
}

}; // namespace SOPSolve
#endif
namespace CNFSolve {

// CNF tseitinFor
CNF numberToOrClause(int number, int literal_start) {
  CNF res;
  for (int i = 0; i < Length; i++) {
    OrClause c;
    Literal l;
    l.idx = literal_start + i;
    l.state = (1 << i) & number ? 1 : 0;
    c.orLiterals.insert(l);
    res.mulClause(c);
  }
  return res;
}

CNF getBlockConstraint(const Step &step) {
  CNF res;
  for (auto v : valid_list) {
    res.addClause(numberToOrClause(v, step.startID));
  }
  return res;
}

CNF getTransConstraint(const Step &step_a, const Step &step_b) {
  CNF res;
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
        CNF a = numberToOrClause(v, step_a.startID);
        CNF b = numberToOrClause(n_v, step_b.startID);
        a.addClause(b);
        res.addClause(a);
      }
    }
  }
  return res;
}

CNF getKthCNF(int k) {
  std::vector<Step> step_list(k);
  for (int i = 0; i < k; i++) {
    step_list[i].startID = i * Length;
    step_list[i].endID = (i + 1) * Length - 1;
  }
  CNF res_1 = getBlockConstraint(step_list[0]);
  for (int i = 1; i < k; i++) {
    res_1.mulClause(getBlockConstraint(step_list[i]));
  }
  CNF res_2 = getTransConstraint(step_list[0], step_list[1]);
  for (int i = 1; i < k - 1; i++) {
    res_2.mulClause(getTransConstraint(step_list[i], step_list[i + 1]));
  }
  res_1.mulClause(res_2);
}

void printCNF(const CNF &s) {
  int i = 0;
  for (auto &vec : s.mulOfClause) {
    int j = 0;
    for (auto &l : vec.orLiterals) {
      if (l.state == 0) {
        printf("not ");
      }
      printf("x%d %s", l.idx, (j++) == vec.orLiterals.size() - 1 ? ")" : "or ");
    }
    printf("%s", (i++) == s.mulOfClause.size() - 1 ? "\n" : "and ");
  }
}

void solve(int n) {
  k = 2 * (n - 1);
  while (k < n * n) {
    CNF s = getKthCNF(k);
    printCNF(s);
  }
}

}; // namespace CNFSolve
int main() {
  std::string file_name = "./bench/1.in";
  freopen(file_name.c_str(), "r", stdin);
  scanf("%d", &n);
  getchar();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      block_map[i][j] = getchar() - '0';
      if (block_map[i][j] == 0) {
        valid_list.insert(i * n + j);
      }
    }
    getchar();
  }
  Length = 1;
  while ((1 << Length) < n) {
    Length += 1;
  }
  CNFSolve::solve(n);
  return 0;
}