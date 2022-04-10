#include "DB.h"
#include <bits/stdc++.h>

bool block_map[100][100];
void solve(int n) {}
int main() {
  string file_name = "./bench/1.in";
  int n;
  freopen(file_name.c_str(), "r", stdin);
  scanf("%d", &n);
  getchar();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      block_map[i][j] = getchar();
    }
    getchar();
  }
  solve(n);
  return 0;
}