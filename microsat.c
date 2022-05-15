
#include "microsat.h"
#include "time.h"

#define MAX_LINE_LEN 80

// Print the model if satisfiable
void print_model(struct solver *S) {

  int n = 2;
  int literal_num = 2;
  int k = 3;
  freopen("./bench/1.args", "r", stdin);
  scanf("%d %d %d", &n, &k, &literal_num);
  char buffer[32];
  size_t current_lit_len;
  // Count the number of characters in the line
  unsigned int line_count = 2;
  printf("v ");
  int end_i = S->nVars / literal_num > k ? k : S->nVars / literal_num;

  for (int i = 0; i < end_i; i++) {
    int start_literal = i * literal_num;
    int num = 0;
    for (int j = 0; j < literal_num; j++) {
      int m = start_literal + j + 1;
      num = num + (S->model[m] << j);
    }
    int xx = num % n;
    int yy = num / n;
    printf("(x:%d ,y:%d)", xx, yy);
    printf("\n");
  }
}

// Print the model if satisfiable
void print_model2(struct solver *S) {
  char buffer[32];
  size_t current_lit_len;
  // Count the number of characters in the line
  unsigned int line_count = 2;
  printf("v ");
  for (int i = 1; i <= S->nVars; i++) {
    sprintf(buffer, "%d", (S->model[i] ? i : -i));
    current_lit_len = strlen(buffer);
    // If max length is exceeded
    if (current_lit_len + line_count > MAX_LINE_LEN) {
      // Including the 'v ' and space after literal
      line_count = current_lit_len + 3;
      printf("\nv %s ", buffer);
    } else {
      line_count += current_lit_len + 1;
      printf("%s", buffer);
      // If there is remaining length for a space
      if (current_lit_len + line_count + 1 <= MAX_LINE_LEN) {
        printf(" ");
      }
    }
  }
  printf("\n");
}

int main(int argc, char **argv) { // The main procedure for a STANDALONE solver
  struct solver S;                // Create the solver datastructure
  clock_t start_time, finish_time;
  start_time = clock();
  if (parse(&S, argv[1]) == UNSAT)
    printf("s UNSATISFIABLE\n"); // Parse the DIMACS file in argv[1]
  else if (solve(&S) == UNSAT)
    printf("s UNSATISFIABLE\n"); // Solve without limit (number of conflicts)
  else {
    // print_model(&S);
    // print_model2(&S);
    printf("s SATISFIABLE\n"); // And print whether the formula has a solution
  }
  finish_time = clock();
  printf(
      "c statistics of %s: mem: %i conflicts: %i max_lemmas: %i time: %f s\n",
      argv[1], S.mem_used, S.nConflicts, S.maxLemmas,
      1. * (finish_time - start_time) / CLOCKS_PER_SEC);
}
