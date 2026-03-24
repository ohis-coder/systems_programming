#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STRIDE_BAL 8
#define STRIDE_SUM 16
#define STRIDE_ADDR 128

void create_account(int id, float initial_bal, const char *name,
                    const char *addr, double *hot_bal, char *sum_row,
                    char *cold_addr) {

  hot_bal[id] = initial_bal;

  char *my_summary = sum_row + (id * STRIDE_SUM);
  float quick_bal = (float)initial_bal;
  memcpy(my_summary, &quick_bal, 4);
  strncpy(my_summary + 4, name, 12);
  char *my_addr = cold_addr + (id * STRIDE_ADDR);
  strncpy(my_addr, addr, STRIDE_ADDR);
}

int main() {
  double balances[1000];
  char addresses[1000 * STRIDE_ADDR];
  char summary[1000 * STRIDE_SUM];
  create_account(500, 1500.50, "Ohimai", "123 Nigeria", balances, summary,
                 addresses);

  printf("Fast Balance check: %.2f\n", balances[500]);
  char *friend_summary = summary + (500 * STRIDE_SUM);
  printf("Friend Preview: %s\n", friend_summary + 4);

  return 0;
}
